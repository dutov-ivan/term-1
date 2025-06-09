#!/bin/bash

cd docs/demo
source .venv/bin/activate
python demo_tests.py
eog test_output_image_1.png
