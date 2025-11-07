from flask import Flask, Response
from flask_cors import CORS

app = Flask(__name__)
cors = CORS(app)

@app.route('/', methods=['GET'])
def tanki():
    resp = Response(open("cmake-build-debug-emsdk//SubterraClient.html").read())
    return resp

@app.route('/SubterraClient.js', methods=['GET'])
def getjs():
    resp = Response(open("cmake-build-debug-emsdk//SubterraClient.js").read(), mimetype="application/javascript")
    return resp

@app.route('/SubterraClient.wasm', methods=['GET'])
def getwasm():
    resp = Response(open("cmake-build-debug-emsdk//SubterraClient.wasm", "rb").read(), mimetype="application/wasm")
    return resp

@app.route('/SubterraClient.data', methods=['GET'])
def getdata():
    resp = Response(open("cmake-build-debug-emsdk//SubterraClient.data", "rb").read(), mimetype="application/octet-stream")
    return resp

app.run(host="localhost", port=3000, threaded=False, debug=True)