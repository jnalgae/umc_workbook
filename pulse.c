#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define RATE_SIZE 10

typedef struct
{
    int channel;
    int BPM;
    int bus;
    int device;
    int max_speed_hz;
} Pulsesensor;

typedef struct
{
    int bus;
    int device;
    // SpiDev 관련 변수는 실제 라이브러리에 따라 선언되어야 합니다.
    // SpiDev spi;
} MCP3008;

void MCP3008_open(MCP3008 *adc)
{
    wiringPiSPISetup(adc->bus, adc->max_speed_hz);
}

int MCP3008_read(MCP3008 *adc, int channel)
{
    int cmd1 = 4 | 2 | ((channel & 4) >> 2);
    int cmd2 = (channel & 3) << 6;

    unsigned char data[3];
    data[0] = cmd1;
    data[1] = cmd2;
    data[2] = 0;

    wiringPiSPIDataRW(adc->bus, data, 3);

    int result = ((data[1] & 15) << 8) + data[2];
    return result;
}

void MCP3008_close(MCP3008 *adc)
{
    // SPI 인터페이스를 닫는 함수
    // wiringPi의 라이브러리에 따라 정의되어야 합니다.
    wiringPiSPIClose(adc->bus);
}

void getBPMLoop(Pulsesensor *sensor, MCP3008 *adc)
{
    int rate[RATE_SIZE] = {0};
    int sampleCounter = 0;
    int lastBeatTime = 0;
    int P = 512;
    int T = 512;
    int thresh = 525;
    int amp = 100;
    int firstBeat = 1;
    int secondBeat = 0;
    int IBI = 600;
    int Pulse = 0;
    int lastTime = (int)time(NULL) * 1000;

    while (1)
    {
        // MCP3008을 통해 센서 값을 읽어옴

        int Signal = MCP3008_read(adc, sensor->channel);
        int currentTime = (int)time(NULL) * 1000;

        // 시간 간격을 측정하여 심장 박동의 타이밍 추적
        // 마지막 측정으로부터 현재까지 경과된 시간, 측정 시간을 계속 누적
        sampleCounter += currentTime - lastTime;
        // lastTiem 업데이트
        lastTime = currentTime;
        int N = sampleCounter - lastBeatTime;

        if (Signal < thresh && N > (IBI / 5.0) * 3)
        {
            if (Signal < T)
            {
                T = Signal;
            }
        }

        if (Signal > thresh && Signal > P)
        {
            P = Signal;
        }

        if (N > 250)
        {
            if (Signal > thresh && Pulse == 0 && N > (IBI / 5.0) * 3)
            {
                Pulse = 1;
                IBI = sampleCounter - lastBeatTime;
                lastBeatTime = sampleCounter;

                if (secondBeat)
                {
                    secondBeat = 0;
                    for (int i = 0; i < RATE_SIZE; i++)
                    {
                        rate[i] = IBI;
                    }
                }

                if (firstBeat)
                {
                    firstBeat = 0;
                    secondBeat = 1;
                    continue;
                }

                for (int i = 0; i < RATE_SIZE - 1; i++)
                {
                    rate[i] = rate[i + 1];
                }
                rate[RATE_SIZE - 1] = IBI;
                int runningTotal = 0;
                for (int i = 0; i < RATE_SIZE; i++)
                {
                    runningTotal += rate[i];
                }
                int averageIBI = runningTotal / RATE_SIZE;
                sensor->BPM = 60000 / averageIBI;
            }
        }

        if (Signal < thresh && Pulse == 1)
        {
            Pulse = 0;
            amp = P - T;
            thresh = amp / 2 + T;
            P = thresh;
            T = thresh;
        }

        if (N > 2500)
        {
            thresh = 512;
            P = 512;
            T = 512;
            lastBeatTime = sampleCounter;
            firstBeat = 1;
            secondBeat = 0;
            sensor->BPM = 0;
        }

        usleep(5000);
    }
}

void startAsyncBPM(Pulsesensor *sensor, MCP3008 *adc)
{
    // 스레드를 시작하여 getBPMLoop 함수를 실행
    pthread_t tid;
    pthread_create(&tid, NULL, (void *)&getBPMLoop, sensor, adc);
}

void stopAsyncBPM(Pulsesensor *sensor)
{
    // 스레드를 중지하고 BPM을 초기화
    sensor->BPM = 0;
}

int main()
{
    Pulsesensor sensor;
    sensor.channel = 0;
    sensor.BPM = 0;

    MCP3008 adc;
    adc.bus = 0;
    adc.device = 0;
    adc.max_speed_hz = 1000000;

    MCP3008_open(&adc);
    startAsyncBPM(&sensor, &adc);

    while (1)
    {
        int bpm = sensor.BPM;
        if (bpm - 155 > 0)
        {
            printf("BPM: %d\n", bpm - 155);
        }
        else
        {
            printf("No Heartbeat found\n");
        }
        usleep(1500000); // 1.5초 대기
    }

    stopAsyncBPM(&sensor);
    MCP3008_close(&adc);

    return 0;
}
