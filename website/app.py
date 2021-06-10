from flask import Flask, render_template
# from database import DataBase

# db = DataBase()

app = Flask(__name__)


@app.route('/')
def index():
    return render_template("index.html")

@app.route('/tempGraph')
def tempGraph():
    # data: time, value
    data = [
    ("13:30", 24.2),
    ("13:50", 24.5),
    ("14:10", 23.9),
    ("14:30", 24.3),
    ]

    labels = [row[0] for row in data]
    values = [row[1] for row in data]

    return render_template("graph_1.html", labels=labels, values=values)

@app.route('/humGraph')
def humGraph():
    # data: time, value
    data = [
    ("13:30", 15),
    ("13:50", 16),
    ("14:10", 17),
    ("14:30", 16),
    ]

    labels = [row[0] for row in data]
    values = [row[1] for row in data]

    return render_template("graph_2.html", labels=labels, values=values)

@app.route('/devices')
def devices():
    return render_template("device.html")

@app.route('/statistics')
def statistics():

    return render_template("statistics.html")

if __name__ == '__main__':
    app.run(debug=True)
