import React from "react";
import { movies } from "./movieDummy.js";
import Movie from "./components/Movie.style.jsx";
import { Body, AppContainer } from './components/Movie.style.jsx';


function App() {
  const func = () => {
    // console.log("clicked");
    // axios({
    //   method: 'post', //메소드
    //   url: 'http://172.21.48.213:8080/api/posts/upload', //엔드포인트
    //     data: {
    //         title : "mockTitle",
    //         author : "mockAuthor",
    //         body : "UMC Server 화이팅!"
    //     }
    // }).then(()=> {
    //   console.log("posted");
    // });
    /* const url =  'https://test.aolda.net/api/posts/upload';
    const data = {
            title : "mockTitle",
            author : "mockAuthor",
            body : "UMC Server 화이팅!"
    }
    axios.post(url, data).then(console.log("posted"));

    const url2 = 'https://test.aolda.net/api/posts/';
    axios.get(url2).then((res)=>{console.log(res.data)});

    const url3 ='https://test.aolda.net/api/posts/2'
    axios.get(url3).then((res)=>{console.log(res.data)}) */
  };

  return (
    <div className="app-container">
      {movies.results.map((item) => {
        return (
          <Body>
            <AppContainer>
              <Movie
              title={item.title}
              poster_path={item.poster_path}
              vote_average={item.vote_average}
              overview={item.overview}
            />
            </AppContainer>
          </Body>
        );
      })}
    </div>
  );
}

export default App;
