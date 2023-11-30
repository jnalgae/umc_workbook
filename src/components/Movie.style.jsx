import React from 'react';
import styled from 'styled-components';

export const Body = styled.body`
  background-color: #0f1472;
`;

export const AppContainer = styled.div`
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
`;

const MovieContainer = styled.div`
  width: 175px;
  margin: 15px;
  background-color: #868ee7;
  color: white;
  font-size: 15px;
  border-radius: 5px;
  overflow: hidden;
`;

const ImageContainer = styled.div`
  position: relative;
  overflow: hidden;
  font-size: 12px;

  &:hover .movie-overview {
    display: block;
  }

  &:hover img {
    filter: brightness(0.3);
    max-width: 100%;
  }
`;

const MovieImage = styled.img`
  width: 175px;
  height: 250px;
`;

const MovieOverview = styled.div`
  display: none;
  position: absolute;
  top: 0;
  left: 5px;
  width: 165px;
  color: white;
  left: 5px;
`;

const MovieInfo = styled.div`
  display: flex;
  justify-content: space-between;
  margin-top: 20px;
  margin-left: 10px;
  margin-right: 10px;
  height: 80px;
`;

const MovieTitle = styled.h4`
  margin-top: 2px;
`;

const MovieVote = styled.span`
  margin-left: 3px;
`;

function Movie({ title, poster_path, vote_average, overview }) {
  return (
    <MovieContainer className='movie-container'>
      <ImageContainer className='image-container'>
        <MovieImage src={"https://image.tmdb.org/t/p/w1280/" + poster_path} alt="poster" />
        <MovieOverview className='movie-overview'>
          <MovieTitle>{title}</MovieTitle>
          <p>{overview}</p>
        </MovieOverview>
      </ImageContainer>

      <MovieInfo className='movie-info'>
        <MovieTitle>{title}</MovieTitle>
        <MovieVote>{vote_average}</MovieVote>
      </MovieInfo>
    </MovieContainer>
  );
}

export default Movie;
