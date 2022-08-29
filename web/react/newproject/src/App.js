import logo from './logo.svg';
import { Component } from 'react';
import './App.css';
import Main from './Components/flag-game.js';
import {Routes, Route, Link} from "react-router-dom";

function App() {
  return (
    <div>
      <header>
        <Link to="/flag-game">Flag Game</Link>
      </header>
    </div>
  );
}

class flagGame extends Component{
  render (){
    return(
      <>
      <Routes>
          <Route path='flag-game' element={flagGame()}/>
       </Routes>
        Main()
      </>
    );
  }
}
export default App;
