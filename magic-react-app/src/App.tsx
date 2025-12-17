import { useState } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from '/vite.svg'
import './App.css'

function LoginForm(){
	return(
		<form>
		<label>
		Username:
		<input type="text" name="username" />
		</label>
		<br />
		<label>
		Password:
		<input type="password" name="password" />
		</label>
		<br />
		<input type="submit" value="Login" />
		</form>
	);
}



function App() {
  return (
    <>
 	<h1>Magic Tutorial</h1>
     	<p>Whether you already love Magic The Gathering and understand all its complexites and want a place to test your deck, or if you need a refresher on what the stack is and how it works, or if you've never touched the game at all, Magic Tutorial is here for you.</p>
	<LoginForm />
    </>
  )
}

export default App