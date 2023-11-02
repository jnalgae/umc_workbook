import { hover } from '@testing-library/user-event/dist/hover'
import React from 'react'

function Movie({title, poster_path, vote_average, overview}) {
  return (
    <div className='movie-container'>
        <div className='image-container'>
          <img src={"https://image.tmdb.org/t/p/w1280/" + poster_path} alt = "poster"/>
          <div className='movie-overview'>
            <h4>{title}</h4>
            <p>{overview}</p>
          </div>
        </div>
        
        <div className='movie-info'>
            <h4>{title}</h4>
            <span>{vote_average}</span>
        </div>
    </div>
  )
}

export default Movie