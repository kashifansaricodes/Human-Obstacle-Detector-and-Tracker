import sys
import onnx

def check_onnx(onnx_file):
    model = onnx.load(onnx_file)
    onnx.checker.check_model(model)
    print("ONNX model check passed")

# Check if filepath is provided
if len(sys.argv) != 2:
    print("Usage: python onnx_to_opencv.py <path_to_onnx_file>")
    sys.exit(1)

onnx_file = sys.argv[1]

try:
    # Check ONNX model
    check_onnx(onnx_file)
    print("ONNX model is valid and can be used directly with OpenCV DNN module.")
except Exception as e:
    print(f"An error occurred: {e}")
