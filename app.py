from flask import Flask, render_template,  jsonify
from mqtt_reader import get_mqtt_message, tempTemp, tempTime, tempHum
from threading import Thread


processGetTemp = Thread(target=get_mqtt_message)
processGetTemp.start()


app = Flask(__name__)

@app.route('/')
def index():
    return render_template("index.html")

@app.route('/getTemp')
def getTemp():
    print("Update web")
    print(tempTemp)
    tview=[s.strftime("%H:%M:%S") if not isinstance(s, int) else 0 for s in tempTime]
    print(tview)
    return jsonify({'tempdata':tempTemp, 'temptime':tview})

@app.route('/getHum')
def getHum():
    print("Update web")
    print(tempHum)
    tview=[s.strftime("%H:%M:%S") if not isinstance(s, int) else 0 for s in tempTime]
    print(tview)
    return jsonify({'tempdata':tempHum, 'temptime':tview})

@app.route('/humGraph')
def humGraph():
    # data: time, value
    labels = [0] * 20
    values = tempHum

    return render_template("graph_2.html", labels=labels, values=values)

@app.route('/tempGraph')
def tempGraph():
    # data: time, value

    labels = [0] * 20
    values = tempTemp

    return render_template("graph_1.html", labels=labels, values=values)

@app.route('/devices')
def devices():
    return render_template("device.html")

@app.route('/statistics')
def statistics():

    return render_template("statistics.html")

if __name__ == '__main__':

    app.run(debug=True)
    processGetTemp.close()
