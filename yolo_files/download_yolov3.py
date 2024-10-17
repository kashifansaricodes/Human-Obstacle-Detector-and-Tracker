import urllib.request
import os

def download_file(url, filename):
    if not os.path.exists(filename):
        print(f"Downloading {filename}...")
        urllib.request.urlretrieve(url, filename)
        print(f"{filename} downloaded.")
    else:
        print(f"{filename} already exists.")

# URLs for the YOLOv3 model files
weights_url = "https://pjreddie.com/media/files/yolov3.weights"
config_url = "https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov3.cfg"
coco_names_url = "https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names"

# Download the files
download_file(weights_url, "yolov3.weights")
download_file(config_url, "yolov3.cfg")
download_file(coco_names_url, "coco.names")

print("Download complete.")
