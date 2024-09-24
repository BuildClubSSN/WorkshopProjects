const net = require('net');
const axios = require('axios'); // To communicate with Python service

// Configuration
const PYTHON_API = 'http://localhost:5000/analyze'; // Python API endpoint

// Create a simple TCP server
const server = net.createServer((socket) => {
    console.log('New connection from:', socket.remoteAddress);

    // Handle incoming data
    socket.on('data', async (data) => {
        const packet = {
            ip: socket.remoteAddress,
            port: socket.remotePort,
            data: data.toString('utf8'), // Convert buffer to string
        };

        console.log('Received packet:', packet);

        // Send packet data to Python firewall for analysis
        try {
            const response = await axios.post(PYTHON_API, packet);
            const result = response.data;

            if (result.block) {
                console.log('Blocked traffic from:', packet.ip);
                socket.destroy(); // Close the connection
            } else {
                console.log('Allowed traffic from:', packet.ip);
            }
        } catch (error) {
            console.error('Error communicating with Python firewall:', error.message);
            socket.destroy();
        }
    });

    // Handle connection close
    socket.on('close', () => {
        console.log('Connection closed:', socket.remoteAddress);
    });

    socket.on('error', (err) => {
        console.error('Connection error:', err.message);
    });
});

// Start the server on port 8080
server.listen(8080, () => {
    console.log('Firewall running on port 8080');
});
