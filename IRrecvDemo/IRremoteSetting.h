
enum namaTombolRemote {
  CHMIN,  CH,  CHPLUS,
  PREV,  NEXT,  PLAY,
  MIN,  PLUS,  EQ,
  NOL,  SERATUS,  DUARATUS,
  SATU,  DUA,  TIGA,
  EMPAT,  LIMA,  ENAM,
  TUJUH,  DELAPAN,  SEMBILAN
};

long tombolRemote[] = {
  0xFFA25D, 0xFF629D, 0xFFE21D,
  0xFF22DD,  0xFF02FD,  0xFFC23D,
  0xFFE01F,  0xFFA857,  0xFF906F,
  0xFF6897,  0x1D320F8B,  0xFFB04F,
  0xFF30CF,  0xFF18E7,  0xFF7A85,
  0xFF10EF,  0xFF38C7,  0xFF5AA5,
  0xFF42BD,  0xFF4AB5,  0xFF52AD
};

#define RECV_PIN  A0
IRrecv irrecv(RECV_PIN);
decode_results results;
#define REMOTEPRESS irrecv.decode(&results)



