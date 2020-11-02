#include "bruteforce-enigma.h"
#include "catch2/catch.hpp"

using namespace std;


// Plaintext: THEENIGMACIPHERWASAFIELDCIPHERUSEDBYTHEGERMANSDURINGWORLDWARIITHEENIGMAISONEOFTHEBETTERKNOWNHISTORICALENCRYPTIONMACHINESANDITACTUALLYREFERSTOARANGEOFSIMILARCIPHERMACHINES
// Unless otherwise noted, ciphertexts were obtained using Cryptii with the indicated settings. Note, Cryptii rotor
//      order is the reverse of this simulator.

// Benchmarks given as a heuristic for expected runtimes. Measured on a i7 2.5Ghz with 4 threads.


// Benchmark: 5s
TEST_CASE("Enigma m3")
{
     Ciphertext ct = "gwydj flpuv shwhs qkepj krwpo vqdcv laknd bpkdl btlju "
                     "xbzeq rmhau myeva pgilt pmoft wzbxp ygwwr sjjff rbgqj "
                     "qygkv ydnfd tvdku ylapv mgfys kuvin pyelw ddlvz opbgv "
                     "tfbmn jpvvt ecnty bsstp tydyv jglfw ynjuk";
     Plugboard plug = "PO ML IU KJ NH YT";

     EnigmaKey expected {ETW_ABCDEF, m3_I, m3_II, m3_III, m3_UKWB, plug};

     auto scores = smart_decipher(m3_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}

// Benchmark: 20s
TEST_CASE("Enigma m3 Kriegsmarine")
{
     Ciphertext ct = "dyrto oawyr nlkux pqexl pztsx siife hcgnq rpmyk fnzmw "
                     "hawxj emrez dsgyl dopya jhytq uuxml osjqz ccrys jauxu "
                     "hgunk yxihm qntxh ievqw xggpf ukmzw upksv mrlev eruaz "
                     "rhjph cdmig vrvro kjksr twnbo bppbh osgun";
     Plugboard plug = "PO ML IU KJ NH YT";

     EnigmaKey expected {ETW_ABCDEF, m3_VI, m3_VII, m3_VIII, m3_UKWB, plug};

     auto scores = smart_decipher(m3_Kriegsmarine_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}


// TODO: Activate test if it gets under 600s.
// // Benchmark:
// TEST_CASE("Enigma m4")
// {
//      Ciphertext ct = "gwyd jflp uvsh whsq kepj krwp ovqd cvla kndb pkdl btlj "
//                      "uxbz eqrm haum yeva pgil tpmo ftwz bxpy gwwr sjjf frbg "
//                      "qjqy gkvy dnfd tvdk uyla pvmg fysk uvin pyel wddl vzop "
//                      "bgvt fbmn jpvv tecn tybs stpt ydyv jglf wynj uk";
//      Plugboard plug = "PO ML IU KJ NH YT";

//      Enigma4Key expected {ETW_ABCDEF, m3_I, m3_II, m3_III, m4_beta, m4_UKWB_thin, plug};

//      auto scores = smart_decipher(m4_model, plug, ct).get_entries();

//      bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
//           return entry.id == expected;
//      });

//      REQUIRE(solution_was_found);
// }

// Benchmark: 7.5s
TEST_CASE("Enigma I")
{
     Ciphertext ct = "xghsg vzecm vdppz bycia rbaza qgrsa vyhjt uvqjb hirih "
                     "mqdwd sftez cfyyl qpwwr mjypr uofyh pqcyd tlqvl kccdp "
                     "tkfgg ezyty cxcph xufmn eysul vvgay urlmv gzkgc pakdj "
                     "gxufm gybhw wogid noahr qpoqo ynkzg wkwsm";
     Plugboard plug = "PO ML IU KJ NH YT";

     EnigmaKey expected {ETW_ABCDEF, m3_I, m3_II, m3_III, m1_UKWA, plug};

     auto scores = smart_decipher(enigma_I_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}

// Benchmark: 2.5s
TEST_CASE("Norenigma")
{
     Ciphertext ct = "fjlqy kvevk hseon sgvct opkqb dsmfc vysbk giatz hedkf "
                     "hbczh blpku narpx zbswo wowhd cbtlh cntvb dfmoj fcayx "
                     "bzzga vgzcf mpqpf vsupl iixoe ybjut qugkc llqzp kxbjh "
                     "nphbm nxcvo pavuj dhumr ldnuv sihep mzevn";
     Plugboard plug = "PO ML IU KJ NH YT";

     EnigmaKey expected {ETW_ABCDEF, norenigma_I, norenigma_II, norenigma_III, norenigma_UKW, plug};

     auto scores = smart_decipher(norenigma_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}

// Benchmark: <1s
TEST_CASE("Sondermaschine")
{
     Ciphertext ct = "fdrcp fkaoi qnmxn gqbzv juiem fjsba guzju icbau wcubd "
                     "gjlgt aazyg hjooq tnuod sohrr sdfmv lmsxw qkqqc sdbhz "
                     "ngpkq yplsw jxjpk xtahl vaapv xybpr lyurl omerj splph "
                     "rmuxg wowwj micdw dugcz mastr aeevz bltaq";
     Plugboard plug = "PO ML IU KJ NH YT";

     EnigmaKey expected {ETW_ABCDEF, sondermaschine_I, sondermaschine_II, sondermaschine_III, sondermaschine_UKW, plug};

     auto scores = smart_decipher(sondermaschine_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}

// Benchmark: <1s
TEST_CASE("Railway")
{
     Ciphertext ct = "afvod ekbxz gebhh eucxv bvayy dzjip xrgty ipnji yphyy "
                     "pxaiq tsmcg sllmp gwspr amoqf hrwqo lasnu nlirq cybbn "
                     "uegga wvqgg efqsx jnqmj cwpka iymbh wwqwg gexoz wqkfq "
                     "pztfh fwwye oazmt xcvid mlzru uhzwq yfqdg";
     Plugboard plug = "";

     EnigmaKey expected {ETW_QWERTZ, railway_I, railway_II, railway_III, railway_UKW, plug};

     auto scores = smart_decipher(railway_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}


// TODO: Get a English plaintext so the test suceeds
// // Benchmark: <1s
// TEST_CASE("CrypTool railway")
// {
//      // Plaintext: IMCWNOVENGAYGHVRXVEBWLYGYHGTGAKTWQOILZSNGVNNTONILINKZTOMBYWUAXQPGAHMOSTXEISUTVAUSSIYV
//      Ciphertext ct = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
//                      "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
//      Plugboard plug = "BY EW FZ GI MQ RV UX";

//      EnigmaKey expected {ETW_QWERTZ, CrypTool_railway_II, CrypTool_railway_I, CrypTool_railway_III, railway_UKW,
//                          plug, 0, 1, 3, 3, 1, 2};

//      auto scores = smart_decipher(CrypTool_railway_model, plug, ct).get_entries();

//      bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
//           return entry.id == expected;
//      });

//      REQUIRE(solution_was_found);
// }

// Benchmark: 30s
TEST_CASE("CrypTool m3")
{
     Ciphertext ct = "gwydj flpuv shwhs qkepj krwpo vqdcv laknd bpkdl btlju "
                     "xbzeq rmhau myeva pgilt pmoft wzbxp ygwwr sjjff rbgqj "
                     "qygkv ydnfd tvdku ylapv mgfys kuvin pyelw ddlvz opbgv "
                     "tfbmn jpvvt ecnty bsstp tydyv jglfw ynjuk";
     Plugboard plug = "PO ML IU KJ NH YT";

     EnigmaKey expected {ETW_ABCDEF, m3_I, m3_II, m3_III, m3_UKWB, plug};

     auto scores = smart_decipher(CrypTool_m3_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}

// Benchmark: <1s
TEST_CASE("Enigma D")
{
     Ciphertext ct = "zdffc qbvyl awfus awyme bvdfo dztrx fubxm pccai akijo "
                     "kerqg gxrca tpjfh fjazw rwtfl zpdiu nrxem wdduw ldtpi "
                     "plksn hrhsj orkgb hsspp ytbbh vueqg oiqzi lhpcr ingju "
                     "skxbv nzyzt nljmi nadjc nxvwy noszo cgpfy";
     Plugboard plug = "";

     EnigmaKey expected {ETW_QWERTZ, commercial_I, commercial_II, commercial_III, commercial_UKW, plug};

     auto scores = smart_decipher(commercial_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}

// Benchmark: 7s
TEST_CASE("Enigma T")
{
     Ciphertext ct = "rvthp uurqi yvmcn sxqme vwora aryha rnwfv mrwvb xcfzk "
                     "rfebj gwtjp yznka acdkk ivlve odciq jpxyw asknd rhlak "
                     "qldne mfogn ixvvu epevh laogv tefbt wkonp dvfkm htbhj "
                     "pgkvk tkptx tnbtj dhane vdsuy tafxj itcid";
     Plugboard plug = "";

     EnigmaKey expected {tirpitz_ETW, tirpitz_I, tirpitz_II, tirpitz_III, tirpitz_UKW, plug};

     auto scores = smart_decipher(tirpitz_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}

// Benchmark: <1s
TEST_CASE("Swiss K")
{
     Ciphertext ct = "komst wjikz kaqsd nlzrq qfhqb oezfx hfurm frros mlxnj "
                     "nngzn lwphu hxvif vdswj stxwk fypnf mjzbd ntorm bxjgv "
                     "khdbw pncsw dqrvb nnouy sarla fedmr waqtn fwsbx ifruv "
                     "qspgk ckglw pjtsm dytcr vuhkr sbyiu ysspp";
     Plugboard plug = "";

     EnigmaKey expected {ETW_QWERTZ, swissK_I, swissK_II, swissK_III, commercial_UKW, plug};

     auto scores = smart_decipher(swissK_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
          return entry.id == expected;
     });

     REQUIRE(solution_was_found);
}

// TODO: find a comparision simulator
// Benchmark:
// TEST_CASE("Enigma KD")
// {
//      Ciphertext ct = "gwydj flpuv shwhs qkepj krwpo vqdcv laknd bpkdl btlju "
//                      "xbzeq rmhau myeva pgilt pmoft wzbxp ygwwr sjjff rbgqj "
//                      "qygkv ydnfd tvdku ylapv mgfys kuvin pyelw ddlvz opbgv "
//                      "tfbmn jpvvt ecnty bsstp tydyv jglfw ynjuk";
//      Plugboard plug = "PO ML IU KJ NH YT";

//      EnigmaKey expected {ETW_ABCDEF, KD_I, KD_II, KD_III, commercial_UKW, plug};

//      auto scores = smart_decipher(KD_model, plug, ct).get_entries();

//      bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
//           return entry.id == expected;
//      });

//      REQUIRE(solution_was_found);
// }


// TODO: Implement the unique behaviors

// Benchmark: <1s
// TEST_CASE("Enigma Z")
// {
//      Ciphertext ct = "yvcgr pxymz dsckz zhwqm hpuug pvaqm vtywq rbrze yuoex "
//                      "zvfjw mcnyl rrfwx kukwt czsih jsnki uuwhs jyarh boobz "
//                      "pwwhl hdzke fjiyk ghnjm ifgpq egepo wlusy qjvjo unnqf "
//                      "lwcyc znpjw cfevb qoxim iqyoi ukknx muxfd";
//      Plugboard plug = "";

//      EnigmaKey expected {ETW_QWERTZ, zahlwerk_I, zahlwerk_II, zahlwerk_III, commercial_UKW, plug};

//      auto scores = smart_decipher(zahlwerk_model, plug, ct).get_entries();

//      bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
//           return entry.id == expected;
//      });

//      REQUIRE(solution_was_found);
// }

// Benchmark: <1s
// TEST_CASE("Enigma G111")
// {
//      Ciphertext ct = "ffyxu hafiz scrgm lgcxu yrcqv nvyce qyxtq pgjzs ueitm "
//                      "gjwem ansnl qhoqb avbip qqrtf fjuia tgwhz igcbl uptxy "
//                      "tbnfd gdofy hgodb lgaws gsdbv vdhxq vwwmw bbazu whbkj "
//                      "kmuqn fguky pydqo tcjdz mkhln llpro turre";
//      Plugboard plug = "";

//      EnigmaKey expected {ETW_QWERTZ, g111_I, g111_II, g111_V, commercial_UKW, plug};

//      auto scores = smart_decipher(g111_model, plug, ct).get_entries();

//      bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
//           return entry.id == expected;
//      });

//      REQUIRE(solution_was_found);
// }

// // Benchmark: <1s
// TEST_CASE("Enigma G312")
// {
//      Ciphertext ct = "epchi nvpgr puqbu fplmw eywib ecskt vqkrw fbbhq sfhwf "
//                      "kqmmz kzfge whkft dgewl gpvbo jgdig tzthc iosfl umqle "
//                      "fekrq gyppl ocjeh zqdms bpocz wakit ytqsv zyxrd xehvk "
//                      "xifop joknp nsuei pbwvk vyqnv zxvhr klqyc";
//      Plugboard plug = "";

//      EnigmaKey expected {ETW_QWERTZ, g312_I, g312_II, g312_III, g312_UKW, plug};

//      auto scores = smart_decipher(g312_model, plug, ct).get_entries();

//      bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
//           return entry.id == expected;
//      });

//      REQUIRE(solution_was_found);
// }

// // Benchmark: <1s
// TEST_CASE("Enigma G260")
// {
//      Ciphertext ct = "iqvsy loiik tnqtq ethgb jbarm uzqcp eubxj grwvn xynmo "
//                      "vrbol jurtp wzbrw dgjkz sluhe qdfjm culmu sojig twzjz "
//                      "sopeb yqhng bcvpv bemku ykvfp nvehp ybrqu yfkah sicbn "
//                      "hngcj fcclw quppu kefqf yqoqm fqijk sjjhq";
//      Plugboard plug = "";

//      EnigmaKey expected {ETW_QWERTZ, g260_I, g260_II, g260_III, commercial_UKW, plug};

//      auto scores = smart_decipher(g260_model, plug, ct).get_entries();

//      bool solution_was_found = std::ranges::any_of(scores, [&] (auto& entry) {
//           return entry.id == expected;
//      });

//      REQUIRE(solution_was_found);
// }

