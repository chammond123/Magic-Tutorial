import { useState } from 'react';

function Login() {
	const [username, setUsername] = useState('');
	const [password, setPassword] = useState('');
	const [error, setError] = useState('');
	const [loading, setLoading] = useState(false);

	const handleSubmit = async (e: React.FormEvent) => {
		e.preventDefault();
		setLoading(true);
		setError('');

		try {
			const response = await fetch('/api/login', {
				method: 'POST',
				headers: { 'Content-Type': 'application/json' },
				body: JSON.stringify({ username, password }),
			});

			if (!response.ok) {
				throw new Error('Login failed');
			}

			const data = await response.json();
			console.log('Login successful:', data);
			// Handle successful login (e.g., redirect, store token, etc.)
		} catch (err) {
			setError(err.message);
		} finally {
			setLoading(false);
		}
	};
	
	return (
		<div className="login form">
		<h2>Login</h2>
		<form onSubmit={handleSubmit}>
		<div>
			<input type="text" placeholder="Username" value={username} onChange={(e) => setUsername(e.target.value)} required />
		</div>
		<div>
			<input type="password" placeholder="Password" value={password} onChange={(e) => setPassword(e.target.value)} required />
		</div>
		<button type="submit" disabled={loading}>{loading ? 'Logging in...' : 'Login'}</button>
		{error && <p className="error">{error}</p>}
		</form>
		</div>

