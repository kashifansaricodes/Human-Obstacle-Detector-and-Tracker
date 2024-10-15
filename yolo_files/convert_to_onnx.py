import torch

# Load the model
model = torch.hub.load('ultralytics/yolov5', 'yolov5s', pretrained=True)
model.eval()

# Create dummy input
dummy_input = torch.randn(1, 3, 640, 640)

# Export to ONNX
torch.onnx.export(model, 
                  dummy_input, 
                  'yolov5s.onnx', 
                  opset_version=11,  # Try a lower opset version
                  input_names=['images'],
                  output_names=['output'],
                  dynamic_axes={'images': {0: 'batch_size'}, 'output': {0: 'batch_size'}})
print("ONNX model saved as yolov5s.onnx")
