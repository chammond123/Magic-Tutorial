// This file sets up authentication using Passport.js with a local strategy. Allowing users to register and log in.
// It uses bcrypt for password hashing and express-session for session management.
// Make sure to install the required packages: express, passport, passport-local, bcrypt, express-session, and body-parser.

const express = require('express');
const router = express.Router();
const bodyParser = require('body-parser');

router.post('/login', (req, res) => {

