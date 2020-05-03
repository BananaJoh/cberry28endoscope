# cberry28endoscope
Live USB endoscope camera monitor on a Raspberry Pi with C-Berry28 display using OpenCV

## Features
* Automatically connect to first camera
* Scale down image resolution to fit display (640x480 capture to 320x240 display)
* Exit on error (combine with autostart loop script below for use with defective USB camera cables)

## Additional Information
* Dependencies:
  * bcm2835 library http://www.airspayce.com/mikem/bcm2835/ (tested with version 1.52)
  * tft.*, ST7789.* and bmp.* files are taken from C-Berry28 software package (see p. 12 of the linked datasheet below)
* Scaled image is converted to bitmap format for the display framebuffer
* 640x480 capture resolution is a tradeoff between framerate and image quality (Raspberry Pi 2B used)
* Backlight PWM value is set to 200 (max. 255)
* For autostart/-restart on boot, e.g. use a crontab entry like this:  
`@reboot while true; do /path/to/cberry28endoscope/endoscope; sleep 10; done`

## Resources
* C-Berry28 datasheet: https://www.admatec.de/pdfs/C-Berry28.pdf
