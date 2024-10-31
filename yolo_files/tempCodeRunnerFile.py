import torch
from models.yolo import DetectionModel  # Import the specific YOLO model
from models.utils import TryExcept, load_image, preprocess_image, postprocess_detections  # Import necessary utils
from pathlib import Path

# Load the model configuration and weights
cfg_path = Path('yolo_files/yolov3.yaml')  # Ensure the path matches your YOLOv3 config file
weights_path = Path('yolo_files/yolov3.weights')  # Specify the path to your weights file

# Create the detection model instance
model = DetectionModel(cfg=cfg_path)

# Load pretrained weights
checkpoint = torch.load(weights_path, map_location=torch.device('cpu'))
model.load_state_dict(checkpoint['model'])

# Set the model to evaluation mode
model.eval()

# Export to ONNX
input_tensor = torch.zeros(1, 3, 640, 640)  # Example input size (1, 3, 640, 640)
onnx_file_path = "yolov3.onnx"
torch.onnx.export(model, input_tensor, onnx_file_path, opset_version=12, 
                  input_names=['input'], output_names=['output'], dynamic_axes={'input': {0: 'batch_size'}, 'output': {0: 'batch_size'}})

print(f"Model successfully exported to {onnx_file_path}")