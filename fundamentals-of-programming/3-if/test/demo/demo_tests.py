import re
import pexpect
from PIL import Image, ImageDraw, ImageFont

def render_ansi_to_image(text, output_image_path):
    # Set the font and size
    font_size = 12
    font = ImageFont.truetype('/usr/share/fonts/JetBrainsMonoNerdFont-Regular.ttf', font_size)
    line_height = 14  # Height per line in pixels

    # Calculate image width and height
    lines = text.splitlines()
    max_line_width = max(len(line) for line in lines) * font_size + 20
    image_height = line_height * len(lines)

    # Create a new image with calculated dimensions
    img = Image.new('RGB', (max_line_width, image_height), color='white')
    draw = ImageDraw.Draw(img)

    # Draw each line, adjusting for ANSI escape codes
    y_text = 10
    for line in lines:
        parts = re.split(r'(\x1B\[[0-?9;]*[mK])', line)
        current_color = (0, 0, 0)  # Default color (black)
        x_text = 10  # Reset x position for each line

        for part in parts:
            if part.startswith('\x1B'):
                # Handle ANSI escape codes to set color
                codes = part[2:-1].split(';')
                for code in codes:
                    if code == '0':  # Reset
                        current_color = (0, 0, 0)
                    elif code == '31':  # Red
                        current_color = (255, 0, 0)
                    elif code == '32':  # Green
                        current_color = (0, 255, 0)
                    elif code == '33':  # Yellow
                        current_color = (255, 255, 0)
                    elif code == '34':  # Blue
                        current_color = (0, 0, 255)
                    elif code == '35':  # Magenta
                        current_color = (255, 0, 255)
                    elif code == '36':  # Cyan
                        current_color = (0, 255, 255)
            else:
                # Draw the text part
                draw.text((x_text, y_text), part, fill=current_color, font=font)
                # Update x_text based on the width of the part using textbbox
                text_bbox = draw.textbbox((0, 0), part, font=font)
                part_width = text_bbox[2] - text_bbox[0]
                x_text += part_width

        # Move y_text down for the next line after drawing all parts of the line
        y_text += line_height

    # Save the image
    img.save(output_image_path)


# Array with inputs for each test as single strings
test_inputs = [
    "abc\n1e20\n1e-20\n17.8\n14\n1e-15\n",  # Test x incorrect cases and 100 14 1e-15 
    "-78\n1e7\n-1e7\nabc\n-2\n-3\n1e-15\n", # Test k incorrect cases and 78 -3 1e-15
    "234.786432\n-123\n2\n1e-20\n1e-15\n", # Test e incorrect cases and 234.786432 -123 1e-15
    "512\n9\n0\n5\n"
]

# Loop through each input in the array
for i, test_input in enumerate(test_inputs):
    TEST_LOG = f"test_output_{i + 1}.txt"

    with open(TEST_LOG, 'w', encoding="utf-8") as log_file:
        log_file.write(f"Running test {i + 1}\n")

        # Start the executable with unicode encoding
        child = pexpect.spawn('../..//build/main.out', encoding='utf-8')

        # Set logfiles to capture all communication
        child.logfile_read = log_file  # Log both sent and received data

        # Split the input into a list
        input_list = test_input.split('\n')

        # Initialize input index
        INPUT_INDEX = 0

        while INPUT_INDEX < len(input_list):
            try:
                # Wait for any prompt (using a more generic pattern)
                child.expect('.*: $', timeout=10)

                # Get the current input
                current_input = input_list[INPUT_INDEX]
                if current_input:  # Only send if it's not empty
                    child.sendline(current_input)
                    INPUT_INDEX += 1

            except pexpect.EOF:
                log_file.write("End of file reached.\n")
                break
            except pexpect.TIMEOUT:
                log_file.write("Timeout occurred while waiting for prompt.\n")
                break

        # Capture any remaining output after sending all inputs
        try:
            child.expect(pexpect.EOF, timeout=10)
            # All output has been logged due to child.logfile
        except pexpect.TIMEOUT:
            log_file.write("Timeout occurred while waiting for EOF.\n")

    # Read the raw log file content with ANSI codes
    with open(TEST_LOG, 'r', encoding="utf-8") as f:
        log_content = f.read()

    # Create a PNG image from the log content
    PNG_OUTPUT = f"test_output_image_{i + 1}.png"
    render_ansi_to_image(log_content, PNG_OUTPUT)

    print(f"Test {i + 1} output saved as PNG: {PNG_OUTPUT}")

print("All tests completed!")
