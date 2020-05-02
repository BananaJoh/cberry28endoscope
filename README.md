# cberry28endoscope
Live picture from a USB endoscope camera on a Raspberry Pi with C-Berry28 display using OpenCV

## Features
* Automatically connect to first camera found
* Scale down image resolution to fit display (640x480 capture to 320x240 display)

## Additional Information
* Scaled image is converted to bitmap format for the display framebuffer
* Backlight value is set to 200 (max. 255)

## Resources
* C-Berry28 datasheet: https://www.admatec.de/pdfs/C-Berry28.pdf
