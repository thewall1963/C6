Import("env")
# Redirect esptool to the Python 3.14 venv that has esptool 5.x dependencies installed
# (pioarduino esptool 5.x requires Python >=3.10 and rich_click, bitstring, etc.)
env.Replace(PYTHONEXE="/Users/frank/.platformio-esptool-venv/bin/python3.14")
