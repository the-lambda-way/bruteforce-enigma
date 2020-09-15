# Enigma Bruteforcer

This is a personal tool I use to bruteforce the decryption of Enigma ciphers, emphasizing speed. It's functional, but not yet in a polished library form. Expect frequent breakage until published as a library. The code has reached fair performance (1.77 x 10^6 decryptions/sec with scoring, 115 characters, Core i7-6500U, 4 threads), so I decided to release it in alpha form as I continue to improve it.


# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.

**Disclosure**: Developed with reference to [CrypTool 2](https://www.cryptool.org/en/cryptool2) and [Practical Cryptography](http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/).

Currently includes some code from Practical Cryptography (*de_qgr.h*, *qgr.h*, *scoreText.c*, *scoreText.h*). I did not find a license, but that code remains under copyright James Lyons © 2009-2012. This project's license will be updated to respect all copyrights as needed.