# Enigma Bruteforcer

This is a personal tool I use to bruteforce the decryption of Enigma ciphers, emphasizing speed. It's functional, but not yet in a polished library form. Expect frequent breakage until published as a library. The code has reached fair performance, so I decided to release it in alpha form as I continue to improve it.

Currently cracks ciphers where the plugboard is already known.


# Usage

Open up **crack_enigma.cpp** and in the main function place your code. Using one of the two provided algorithms, pass in an enigma model, a plugboard setting, and your ciphertext. Compile by running `make` or `make crack_enigma` and run with `./crack_enigma`.

To import into an existing project, drop the repo into your source tree, include include/bruteforce-enigma.h in a file, and compile and link with the .cpp files in src/. You can only compile against one quadgram language file (qgr.cpp or de_qgr.cpp) at a time.

Enigma models can be found in *models.cpp*. The current list includes:
* m3
* m3 extended (with rotors VI, VII, and VIII added)
* m4 (with only three rotors at the moment)
* railway
* CrypTool m3 (adds reflector UKWA)
* CrypTool railway (different notches)
* enigma I
* norenigma
* sondermaschine
* commercial enigma

Two algorithms are provided to bruteforce an enigma cipher. The first, called `smart_decipher`, should be sufficient for almost all ciphers. It uses a similar algorithm to [Practical Cryptography](http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/), by first finding the best rotor combination and position, and then from the top results finding the best ring positions.

The second, called `bf_decipher`, goes through every combination of rotor, rotor position, and ring position (except the third ring, which has no effect on finding a solution) of a given enigma model. Since this requires 11.9 million decryptions per rotor configuration, it takes a bit of time to finish. See below for the performance characteristics on my machine.

Two different scoring functions are provided, `score_by_Qgram` for quadgram scoring, and `score_by_IOC_order` for IOC scoring (maintains ordering, but does not return the actual IOC).



# Performance

Test using quadgram scoring on my personal laptop, with a 56-character cipher from Practical Cryptography.

```
ct: NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB
pt: INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN
```

From: http://practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma/


| Core i7-6500U, 4 threads       | smart_decipher        | bf_decipher           |
| ------------------------------ | ----------------------| ----------------------|
| Cipher length                  | 56                    | 56                    |
| Enigma model                   | m3 (250 combinations) | m3 (250 combinations) |
| Solve time                     | 1.75 sec              | 13 min, 37 sec        |
| Number of decryptions / second | -                     | 3.63 x 10^6           |
| Characters / second            | -                     | 203.52 x 10^6         |



Scoring with IOC is faster than quadgram, but also is less accurate. IOC scoring fails to solve the above cipher, as due to the plaintext's short length, its IOC is 0.080519, while the IOC scorer looks for the average English score of 0.066.

A longer cipher test was done on a text with IOC of 0.062931, which is much closer to the average English score. Even so, the smart_decipher function fails to solve the longer text, likely because IOC does not score well on the partial solutions found during the first part of the decryption process.

```
ct: YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA
    JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ
    VMREGWSBFZBM
pt: THEENIGMACIPHERWASAFIELDCIPHERUSEDBYTHEGERMANSDURINGWORLDWARIITHEENIGMAISONEOFT
    HEBETTERKNOWNHISTORICALENCRYPTIONMACHINESANDITACTUALLYREFERSTOARANGEOFSIMILARCI
    PHERMACHINES
```

From: http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/

| Core i7-6500U, 4 threads       | bf_decipher           |
| ------------------------------ | --------------------- |
| Cipher length                  | 170                   |
| Enigma model                   | m3 (250 combinations) |
| Solve time                     | 34 min, 51 sec        |
| Number of decryptions / second | 1.42 x 10^6           |
| Characters / second            | 241.53 x 10^6         |



# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.

Developed with reference to [CrypTool 2](https://www.cryptool.org/en/cryptool2) and [Practical Cryptography](http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/).

Currently includes some code from Practical Cryptography (*de_qgr.h*, *qgr.h*). I did not find a license, but that code remains under copyright James Lyons © 2009-2012. This project's license will be updated to respect all copyrights as needed.

Some enigma model definitions are taken from [Cryptii](https://github.com/cryptii/cryptii), which is under the MIT license.
