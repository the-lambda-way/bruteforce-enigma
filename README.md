
# Enigma Bruteforcer

A tool for bruteforcing enigma ciphers. Currently breaks ciphers where the plugboard is already known.


# Installation

Clone or download the project. Open up `crack_enigma.cpp` and in the `main` function place your code (see Basic Usage below). Compile by running `make` or `make crack_enigma` and run with `./crack_enigma`.

To import into an existing project, you need the the contents of the `source/` and `include/` folders. `include/bruteforce-enigma.h` adds a nicer interface and multithreading, but doesn't contain any core functionality. Compile and link with the .cpp files from `source/`, but you can only compile against one quadgram language file (`qgr.cpp` or `de_qgr.cpp`) at a time. Requires compiling with C++20 and pthreads.


# Basic Usage

**Decipher with a known setting**
```Cpp
Ciphertext ct = "odlio qsapd tbttn ueokz jbqzw hlynd mmbvn dlanx tvrny lvqmj kovre v";
Plugboard plug = "bq cr di ej kw mt os px uz gh";

Enigma enigma {ETW_ABCDEF, m3_I, m3_III, m3_I, m3_UKWB, plug, "BLQ", "BEC"};

Ciphertext pt = enigma.encrypt(ct);
std::cout << pt;
```


To decipher an m4 model, replace `Enigma` above with `Enigma4`, like so.
```Cpp
Enigma4 enigma {ETW_ABCDEF, m3_I, m3_III, m3_I, m4_beta, m4_UKWB_thin, plug, "BLQA", "BECA"};
```


**Cracking unknown ring and rotor positions**

The smart decipher should be sufficient for almost all ciphers. It uses a similar algorithm that [Practical Cryptography](http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/) does, by first finding the best rotor combination and position, and then from the top results finding the best ring positions.

```Cpp
Ciphertext ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
Plugboard plug = "";

EnigmaBase base = {ETW_ABCDEF, m3_IV, m3_V, m3_II, UKWB};

HighScores scores = smart_decipher(base, plug, ct);
scores.print();
```


**Bruteforcing unknown ring and rotor positions**

The bruteforce decipher goes through every combination of rotor, rotor position, and ring position (except the third ring, which has no effect on finding a solution) of a given enigma model. Since this requires 11.9 million decryptions per rotor configuration, it takes a bit of time to finish. See below for the performance characteristics on my machine.

```Cpp
Ciphertext ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
Plugboard plug = "";

EnigmaBase base = {ETW_ABCDEF, m3_IV, m3_V, m3_II, UKWB};

HighScores scores = bruteforce_decipher(base, plug, ct);
scores.print();
```


**Cracking unknown rotors**

Pass in one of the models in `models.h`.

```Cpp
Ciphertext ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
Plugboard plug = "";

HighScores scores = smart_decipher(m3_model, plug, ct);
scores.print();
```


**Bruteforcing unknown rotors**

```Cpp
Ciphertext ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
Plugboard plug = "";

HighScores scores = bruteforce_decipher(m3_model, plug, ct);
scores.print();
```


**Changing the scoring model**

Two different scoring functions are provided, `score_by_Qgram` for quadgram scoring (the default), and `score_by_IOC_order` for IOC scoring (maintains ordering, but does not return the actual IOC). You can also provide a custom scoring function, which must take a range of integers as its only parameter.

```Cpp
Ciphertext ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
Plugboard plug = "";

HighScores scores = smart_decipher(m3_model, plug, ct, score_by_IOC_order);
scores.print();
```


**Increasing the number of scores**

If you suspect a solution is missed because it is not in the top 10 results (rare), you can increase the number of high scores with a template argument. The smart decipher runtime will be increased by (26 + N)/36 %. The effect on the bruteforce runtime is negligible.

```Cpp
Ciphertext ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
Plugboard plug = "";

HighScores scores = smart_decipher<25>(m3_model, plug, ct);
scores.print();
```



# Available Models

Enigma model definitions can be found in `models.cpp`. The list includes:
* M3
* Kriegsmarine (M3 with rotors VI, VII, and VIII added)
* M4
* Enigma I
* Norenigma
* Sondermaschine
* Railway
* CrypTool Railway (different notches and stator)
* CrypTool M3 (adds reflector UKWA to Kriegsmarine)
* Commercial Enigma
* Tirpitz

The following rotor definitions are included, but the unique machine behaviors have not been implemented.
* Swiss K
* Enigma KD
* Zahlwerk
* Enigma G-111
* Enigma G-312
* Enigma G-260


# Advance Usage

Quadgram scoring for German can be found in `de_qgr.h` and `de_qgr.cpp`. There is currently no mechanism to specify the language at runtime, but you can change the import in `score.h` from `qgr.h` to `de_qgr.h`, recompile, and it will just work.


# Performance

Test using quadgram scoring on my personal laptop, with a 56-character cipher from Practical Cryptography.

```
ct: NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB
pt: INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN
```

From: http://practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma/


| Core i7 2.5GHz, 4 threads      | smart_decipher        | bruteforce_decipher   |
| ------------------------------ | ----------------------| ----------------------|
| Cipher length                  | 56                    | 56                    |
| Enigma model                   | M3 (250 combinations) | M3 (250 combinations) |
| Solve time                     | 1.6 sec               | 13 min, 23 sec        |
| Number of decryptions / second | -                     | 3.70 x 10^6           |
| Characters / second            | -                     | 207.16 x 10^6         |


Scoring with IOC is faster than quadgram, but is also less accurate. It fails to solve the cipher above because its IOC is 0.080519, while the average English IOC is 0.066.

The text below has an IOC of 0.062931, which is much closer to English. Even so, the smart_decipher function fails to solve the longer text, possibily because IOC does not score well on the partial solutions found during the first part of the decryption process.

```
ct: YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA
    JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ
    VMREGWSBFZBM
pt: THEENIGMACIPHERWASAFIELDCIPHERUSEDBYTHEGERMANSDURINGWORLDWARIITHEENIGMAISONEOFT
    HEBETTERKNOWNHISTORICALENCRYPTIONMACHINESANDITACTUALLYREFERSTOARANGEOFSIMILARCI
    PHERMACHINES
```

From: http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/

| Core i7 2.5GHz, 4 threads      | bruteforce_decipher   |
| ------------------------------ | --------------------- |
| Cipher length                  | 170                   |
| Enigma model                   | M3 (250 combinations) |
| Solve time                     | 33 min, 21 sec        |
| Number of decryptions / second | 1.48 x 10^6           |
| Characters / second            | 252.30 x 10^6         |


# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.

Developed with reference to [CrypTool 2](https://www.cryptool.org/en/cryptool2), [Practical Cryptography](http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/), and [Crypto Museum](https://www.cryptomuseum.com/).

Currently includes some code from Practical Cryptography (*de_qgr.h*, *qgr.h*). I did not find a license, but that code remains under copyright James Lyons © 2009-2012. This project's license will be updated to respect all copyrights as needed.

Some enigma model definitions are taken from [Cryptii](https://github.com/cryptii/cryptii), which is under the MIT license.
