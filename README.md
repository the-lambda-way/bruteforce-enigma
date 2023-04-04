
# Enigma Bruteforcer

A tool for cracking and bruteforcing ciphertext-only enigma ciphers. The ultimate aim of this tool is to be as fast as possible on a household multi-core machine.

Currently breaks ciphers where the plugboard is already known.

Best [performance](#Performance) so far:

| Core i7 2.5GHz, 4 threads | 252.30 million chars / second |
|-|-|


# Installation

Clone or download the project. Open up `crack_enigma.cpp` and in the `main` function place your code (see [Usage](#Usage) below). Compile by running `make` or `make crack_enigma` and run with `./crack_enigma`.

To import into an existing project, you need the the contents of the `source/` and `include/` folders. `include/bruteforce-enigma.h` adds a nicer interface and multithreading, but doesn't contain any core functionality. Compile and link with the .cpp files from `source/`, but you can only compile against one quadgram language file (`qgr.cpp` or `de_qgr.cpp`) at a time. Requires compiling with C++20 and pthreads.

# Table of Contents

[About](#About)<br/>
[Usage](#Usage)<br/>
[Performance](#Performance)<br/>
[Future Plans](#Future-Plans)<br/>
[Why I created this tool](#Why-I-created-this-tool)<br/>
[License](#License)

# About

This tool assumes a working knowledge of enigma terminology (there is no wizard). Some good resources I have used for learning about enigma are [Wikipedia](https://en.wikipedia.org/wiki/Enigma_machine), [Practical Cryptography](http://practicalcryptography.com/ciphers/enigma-cipher/), and [Crypto Museum](https://www.cryptomuseum.com/crypto/enigma/).

It also expects basic C++ programming knowledge so that you can modify the source code for your particular needs. There is no command line interface. I haven't written documentation yet, so you're expected to read the code if you're going beyond the most basic usage.


## Features
* Decipher with a known setting
* Crack ring or rotor positions
* Crack rotors
* Quick solve or full bruteforce
* Score by IOC, quadgram, or a custom scoring function
* Change the number of top scores considered while breaking


## Flaws
* Does not support the use of cribs
* Does not crack plugboards
* Does not support pausing and resuming a decryption
* Does not progressively show the scores (it slows down the decryption)
* Wheel order is in reverse and numerical positions start at 0 (I have to fix these to match the norm)


## Available Models

Enigma model definitions can be found in `models.cpp`. The list includes:
* M3 (rotors I, II, III, IV, and V)
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


## Compatibility with Other Enigma Tools

It is often helpful to compare several enigma decrypters at the same time. Below is a list of some decrypters I know of. Most online tools follow the wheel order of the historical Enigma, which runs right to left. This one doesn't, so you'll have to reverse the order of the rotors you're testing. Additionally, most online tools start ring and rotor positions at 1 (if not using letters) while this tool starts at 0.

* [cryptii](https://cryptii.com/)
* [CrypTool-Online](https://www.cryptool.org/en/cto/enigma-step-by-step)
* [dCode](https://www.dcode.fr/enigma-machine-cipher)
* [Enigma Simulator](https://www.ciphermachinesandcryptology.com/en/enigmasim.htm) (Dirk Rijmenants)
* [Practical Cryptography](http://practicalcryptography.com/ciphers/enigma-cipher/)
* [pycipher](https://github.com/jameslyons/pycipher)
* [Universal Enigma](http://people.physik.hu-berlin.de/~palloks/js/enigma/enigma-u_v26_en.html)


# Usage

The abstract signature is

`Enigma(stator, rotor1, rotor2, rotor3, reflector, plugboard, rotor_positions, ring_positions)`

For the concrete signatures see *bruteforce-enigma.h*. The model names are located in *models.h* (definitions in *models.cpp*).


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

EnigmaBase base {ETW_ABCDEF, m3_IV, m3_V, m3_II, m3_UKWB};

HighScores scores = smart_decipher(base, plug, ct);
scores.print();
```


**Bruteforcing unknown ring and rotor positions**

The bruteforce decipher goes through every combination of rotor, rotor position, and ring position (except the third ring, which has no effect on finding a solution) of a given enigma model. Since this requires 11.9 million decryptions per rotor configuration, it takes a bit of time to finish. See below for the performance characteristics on my machine.

```Cpp
Ciphertext ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
Plugboard plug = "";

EnigmaBase base {ETW_ABCDEF, m3_IV, m3_V, m3_II, UKWB};

HighScores scores = bruteforce_decipher(base, plug, ct);
scores.print();
```


**Cracking unknown rotors**

Pass in one of the models in `models.h`.

(Note: m3_model uses rotors I - V. For all rotors I - VIII, use kriegsmarine_model.)

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

Two different scoring functions are provided, `score_by_Qgram` for quadgram scoring (the default), and `score_by_IOC_order` for IOC scoring (maintains ordering, but does not return the actual IOC; this increases performance). You can also provide a custom scoring function, which must take a range of integers as its only parameter.

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

**Changing to German Language**

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

# Future Plans
In no order:
* Fix the rotor order and position ordinals to match other tools.
* Documentation.
* Investigate the correct double-stepping behavior when the middle and leftmost rotor positions begin at their notches. If that is a bug in this program, fix it. If not, document the behavior because a lot of enigma tools behave differently.
* More detailed comments and/or public internal documentation.
* Look into using named parameters (they're pretty limited in C++ so we'll see if it will even be an improvement).
* Add a namespace and other library code hygiene practices.
* Keep improving the parallelization. Enigma is highly parallelizable so this will enable future optimizations. Hopefully someday taking advantage of GPUs.
* Various optimization ideas spread through the code comments that I intend to explore.
* Add a command line interface.
* Implement unique Enigma variations.
* Add a second encryption step (user-supplied).
* Crack the plugboard.
* Read enigma research papers and implement any useful algorithms. This codebase is fairly modular so it should be possible to let users choose an algorithm (if there are any tradeoffs).
* Incorporate cribs? I haven't decided yet.



# Why I created this tool
I created this tool while solving video game ciphers where the cipher type is unknown. Solving for unknown cipher types often involves experimenting with many different algorithms and many different settings. Enigma has a large amount of possible settings, so it can take a long time to test them all. I don't want to waste time using unoptimized decrypters when I'm not sure I'm even close to a solution yet.

There are many good enigma resources on the internet. But bottom line, I couldn't find any available code repository that cracks a ciphertext-only enigma cipher on a single home computer in an optimized manner. The fastest enigma crackers I found are designed for distributed solving, which doesn't fit my use case.

I also found many good academic papers describing fast algorithms. Most don't make a working program available, and some are locked behind a paywall.

Fast algorithms are necessary for practical solving on a single machine. However, machine-oriented optimizations are often equally as important as the choice of algorithm. As a low-level programmer, I thought I could contribute something meaningful in this area. Enigma seems well-suited to benefit from these kinds of optimizations. Since it has millions of rotor-related combinations and trillions of plugboard combinations, even small optimizations at the single decryption level can shave minutes off the total time.

It is my hope that this code will introduce optimizations that can improve the quality of enigma implementation across the internet, or possibly in the future serve as a core library (after I do some necessary refactoring).

# License

Copyright (c) 2020 Mike Castillo under the [MIT License](https://choosealicense.com/licenses/mit/). See LICENSE for the full terms.

Developed with reference to [CrypTool 2](https://www.cryptool.org/en/cryptool2), [Practical Cryptography](http://www.practicalcryptography.com/cryptanalysis/breaking-machine-ciphers/cryptanalysis-enigma-part-2/), and [Crypto Museum](https://www.cryptomuseum.com/).

Currently includes some code from Practical Cryptography (*de_qgr.h*, *qgr.h*). I did not find a license, but that code remains under copyright James Lyons © 2009-2012. This project's license will be updated to respect all copyrights as needed.

Some enigma model definitions are taken from [Cryptii](https://github.com/cryptii/cryptii), which is under the MIT license.
