// This file sets up a simple Express server that servers game commands and data using websockets. The server listens on port 3000 and receives commands from clients to control a game. Those commands are sent to the c++ backend, the backend validateds and processes them, and updates the game state. The server then sends the updated game state back to the clients.

const express = require('express');
const bodyParser = require('body-parser');
const webSocket = require('ws');
const http = require('http');
const passport = require('passport');
const LocalStrategy = require('passport-local').Strategy;
const bcrypt = require('bcrypt');
const session = require('express-session');

cosnt app = express();
const server = http.createServer(app);

const wss = new webSocket.Server({ server });

app.use(bodyParser.json());

// In-memory user store for testing
const users = []

passport.use(new LocalStrategy(
	async (username, password, done) => {
		const user = users.find(u => u.username === username);
		if !user return done(null, false, { message: 'Incorrect username.' });
		const match = await bcrypt.compare(password, user.password);
		if (!match) return done(null, false, { message: 'Incorrect password.' });
		return done(null, user);
	}
));

passport.serializeUser((user, done) => {
	done(null, user.id);
});

passport.deserializeUser((id, done) => {
	const user = users.find(u => u.id === id);
	done(null, user);
});

app.use(session({
	secret: 'your_secret',
	resave: false,
	saveUninitialized: false
}));

app.use(passport.initialize());
app.use(passport.session());



wss.on('connection', (ws) => {

    console.log('Client connected');

    ws.on('message', (message) => {
	console.log(`Received message: ${message}`);
// This file contaisn the server code for the communication between the client and the game server. 
// It handles the connection, disconnection, and message events.

const express = require('express');
const app = express();
const http = require('http');
const port = 8080;

const server = http.createServer(app);


	// Simulating a response from the C++ backend
	const response = `Processed command: ${message}`;

	// Send the response back to the client
	ws.send(response);
    });

    ws.on('close', () => {
	console.log('Client disconnected');
    });
}
