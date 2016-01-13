#!/usr/bin/env python

import os
from uuid import uuid4

from flask import Flask, request, url_for, send_from_directory
from werkzeug import secure_filename

app = Flask(__name__)
app.config.from_pyfile('config.py')


@app.route('/')
def hello_world():
    return 'Welcome to Screen.me project!'


@app.route('/upload', methods=['GET', 'POST'])
def upload():
    if request.method == 'POST':
        f = request.files['image']
        if f and allowed(f.filename):
            filename = secure_filename(f.filename)
            filename = str(uuid4())[:8] + '.' + f.filename.rsplit('.', 1)[1]

            if not os.path.exists(app.config['UPLOAD_FOLDER']):
                os.mkdir(app.config['UPLOAD_FOLDER'])

            f.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))

            return app.config['HOST'] + url_for('get_uploaded',
                                                filename=filename)

    else:
        return 'file uploading page'


@app.route('/uploaded/<filename>')
def get_uploaded(filename):
    return send_from_directory(app.config['UPLOAD_FOLDER'], filename)


def allowed(filename):
    return ('.' in filename and
            filename.rsplit('.', 1)[1] in app.config['ALLOWED_EXTENSIONS'])

if __name__ == '__main__':
    app.run()
