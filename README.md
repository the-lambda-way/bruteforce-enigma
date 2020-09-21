# Enigma Bruteforcer

This is a personal tool I use to bruteforce the decryption of Enigma ciphers, emphasizing speed. It's functional, but not yet in a polished library form. Expect frequent breakage until published as a library. The code has reached fair performance, so I decided to release it in alpha form as I continue to improve it.

Currently cracks ciphers where the plugboard is already known.


# Usage

Open up **crack_enigma.cpp** and in the main function place your code. Using one of the two provided algorithms, pass in an enigma model, a plugboard setting, and your ciphertext. Compile by running `make` or `make crack_enigma` and run with `./crack_enigma`. Takes about 7 seconds to compile on my machine. You could precompile the headers to speed this up if you want.

Two enigma models are currently provided, m3 and railway. Look in *models.h* to examine the definitions. A [CrypTool2](https://www.cryptool.org/de/cryptool2) variant of railway is provided, since it uses different notches than the standard.

Two algorithms are provided to bruteforce an enigma cipher. The first, called `smart_decipher`, should be sufficient for almost all ciphers. It uses a similar algorithm as [practical cryptography](http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/), by first finding the best rotor combination and position, and then from the top results finding the best ring positions.

The second, called `bf_decipher`, goes through every combination of rotor, rotor position, and ring position of a given enigma model. This algorithm is useful for very short messages, or complex ones that wouldn't score well using a quadgram scoring model. Since this requires over 300 million decryptions per rotor configuration, it can take a very long time to finish. See below for the performance characteristics on my machine.



# Performance

Tested on my personal laptop, with a 56-character cipher.



| Core i7-6500U, 4 threads       | smart_decipher            | bc_decipher               |
| ------------------------------ | ------------------------- | ------------------------- |
| Cipher length                  | 56                        | 56                        |
| Enigma model                   | railway (27 combinations) | railway (27 combinations) |
| Total time                     | 0.75 sec                  | 59 min, 30 sec            |
| Number of decryptions / second | -                         | 2.34 x 10^6               |
| Characters / second            | -                         | 130.83 x 10^6             |




# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.

**Disclosure**: Developed with reference to [CrypTool 2](https://www.cryptool.org/en/cryptool2) and [Practical Cryptography](http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/).

Currently includes some code from Practical Cryptography (*de_qgr.h*, *qgr.h*, *scoreText.c*, *scoreText.h*). I did not find a license, but that code remains under copyright James Lyons © 2009-2012. This project's license will be updated to respect all copyrights as needed.

Some engima model definitions are taken from [Cryptii](https://github.com/cryptii/cryptii), which is under the MIT license.