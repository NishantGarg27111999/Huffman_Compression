import os
import time
import glob
from flask import Flask, redirect, render_template, request, send_file

# Configure Application
app = Flask(__name__)

global filename
global ftype

@app.route("/")
def home():

    # Delete old files
    filelist = glob.glob('uploads/*')
    for f in filelist:
        os.remove(f)
    filelist = glob.glob('downloads/*')
    for f in filelist:
        os.remove(f)
    return render_template("home.html")

app.config["FILE_UPLOADS"] = "./uploads"

@app.route("/compress", methods=["GET", "POST"])
def compress():

    if request.method == "GET":
        return render_template("compress.html", check=0)

    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('c uploads/{}'.format(filename))
            print("mein chala")
            filename = filename[:filename.index(".",1)]
            print(filename)
            ftype = "-compressed.bin"
            while True:
                if 'uploads/{}-compressed.bin':
                    print("mein download k liye")
                    os.system('move uploads\{}-compressed.bin downloads'.format(filename))
                    break

            return render_template("compress.html", check=1)

        else:
            print("ERROR")
            return render_template("compress.html", check=-1)

@app.route("/decompress", methods=["GET", "POST"])
def decompress():

    if request.method == "GET":
        return render_template("decompress.html", check=0)

    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system('d uploads/{}'.format(filename))
            f = open('uploads/{}'.format(filename), 'rb')
            ftype = "-decompressed.txt" 
            filename = filename[:filename.index("-",1)]
            print(filename)
            print(ftype)
            while True:
                if 'uploads/{}-decompressed.txt':
                    print("decompress mein hun main")
                    os.system('move uploads\{}-decompressed.txt downloads'.format(filename))
                    break

            return render_template("decompress.html", check=1)

        else:
            print("ERROR")
            return render_template("decompress.html", check=-1)







@app.route("/download",methods=["GET", "POST"])
def download_file():
    global filename
    global ftype
    print(filename)
    print(ftype)
    path = "downloads/" + filename + ftype
    return send_file(path, as_attachment=True)




# Restart application whenever changes are made
if __name__ == "__main__":
    app.run(debug = True)