
This guide demonstrates how to build a simple firewall system using Node.js and Python. It simulates traffic monitoring, packet filtering, and basic rule enforcement.

## 1. Project Structure

- **Node.js**: Handles incoming traffic and communicates with the Python service to analyze packets.
- **Python**: Implements firewall rules and checks packets for blocked IPs, ports, and suspicious data.

## 2. Prerequisites

- Node.js
- Python 3.x
- Flask (for the Python service)

Install the required packages:
```bash
# Node.js dependencies
npm install net axios

# Python dependencies
pip install flask
```

## 3. Code

### Node.js: Handling Incoming Traffic

```javascript
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
```

### Python: Firewall Rule Processing

```python
from flask import Flask, request, jsonify

app = Flask(__name__)

# Sample list of blocked IPs and ports
blocked_ips = ['192.168.0.100', '10.0.0.5']
blocked_ports = [22, 8081]  # Example: block SSH and another custom port
blocked_keywords = ['attack', 'malicious']

def check_packet(packet):
    ip = packet['ip']
    port = packet['port']
    data = packet['data'].lower()

    # Block if the IP is in the blocked list
    if ip in blocked_ips:
        return True

    # Block if the port is in the blocked list
    if port in blocked_ports:
        return True

    # Block if any keyword from blocked_keywords is in the data
    for keyword in blocked_keywords:
        if keyword in data:
            return True

    return False

@app.route('/analyze', methods=['POST'])
def analyze_packet():
    packet = request.get_json()
    block = check_packet(packet)
    
    # Return the result: block = True/False
    return jsonify({'block': block})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
```

## 4. Running the System

1. Start the Python firewall service:
   ```bash
   python firewall.py
   ```

2. Start the Node.js traffic handler:
   ```bash
   node firewall.js
   ```

## 5. Testing the Firewall

### Method 1: Using Telnet

You can use `telnet` to simulate network traffic:
```bash
telnet localhost 8080
```

Send a message:
```
hello from the client
```

### Method 2: Node.js Client

You can also create a simple client to send packets to the firewall:
```javascript
const net = require('net');

const client = new net.Socket();

client.connect(8080, '127.0.0.1', () => {
    console.log('Connected to the firewall');
    client.write('This is a clean packet'); // Test with other content like 'attack'
});

client.on('data', (data) => {
    console.log('Received:', data.toString());
    client.destroy(); // Close after receiving response
});

client.on('close', () => {
    console.log('Connection closed');
});
```

## 6. Enhancements/Homework

- **Dynamic Rule Management**: Allow adding or removing IPs, ports, or keywords dynamically.
- **Logging**: Implement logging of all blocked and allowed packets for audit purposes.
- **Time-based Blocking**: Block traffic based on specific times of the day.