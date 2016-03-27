import stego_py
from distutils.dir_util import copy_tree

import random, string

def randomword(length):
    return ''.join(random.choice(string.ascii_lowercase) for i in range(length))

from_directory = "StegoDisk/src/tests/img"
to_directory = "/tmp/stego_py_test"

copy_tree(from_directory, to_directory)

stego_disk = stego_py.StegoStorage()

stego_disk.open(to_directory, 'heslo')
# stego_disk.configure()
stego_disk.configure(stego_py.encoder.hamming, stego_py.permutation.feistel_mix, stego_py.permutation.feistel_mix)
stego_disk.load()

in_buffer = randomword(stego_disk.get_size()).encode('ascii')

stego_disk.write(in_buffer)

out_buffer = stego_disk.read(stego_disk.get_size())

stego_disk.save()

if out_buffer == in_buffer :
    print(":)")
    else :
        print(":(")
