# studless
Lego model viewer

# obtaining LDraw library

curl --output ldraw.zip https://www.ldraw.org/library/updates/complete.zip
unzip ldraw.zip

# obraining ldraw.xml
Read info at https://wiki.ldraw.org/wiki/LDraw.xml
curl -O http://slswww.free.fr/ldraw.xml

# make library hash

./mkhash.pl --ldraw=./ldraw




