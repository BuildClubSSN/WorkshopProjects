from flask import Flask, request, jsonify

app = Flask(__name__)

# Sample list of blocked IPs and ports (can be expanded)
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
