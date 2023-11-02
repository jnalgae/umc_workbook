import React from "react";
import { movies } from "./movieDummy.js";
import Movie from "./components/Movie.jsx"; 

function App() {
  return (
    <div className="app-container">
      {
        movies.results.map((item) => {
          return (
          <Movie
            title={item.title}
            poster_path = {item.poster_path}
            vote_average = {item.vote_average} 
            id = {item.id}
            overview={item.overview}
          />
          )
        })
      }
    </div>
  );
}

export default App;

