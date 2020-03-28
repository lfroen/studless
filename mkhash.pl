#!/usr/bin/perl -w

use Getopt::Long;

#my $ldraw = $ARGV[0];
#die "ERROR - no ldraw dir" unless defined $ldraw;

my $ldraw = undef;
my $libname = 'library.bin';
my $mph = "/Users/lfroen/Documents/proj/mylego/studless/mph-1.2";
my $nodata = 0;
my $nohash = 0;
my $printfiles = 0;
my $maxfiles = 0x7fffffff;

GetOptions(
    "ldraw=s"     => \$ldraw,
    "libname=s"   => \$libname,
    "nodata"      => \$nodata,
    "maxfiles=i"  => \$maxfiles,
    "nohash"      => \$nohash,
    "printfiles"  => \$printfiles,
    "mph=s"       => \$mph)
or die("Error in command line arguments\n");
die "No LDraw directory in options" unless defined $ldraw;

printf "Generate hash: %s, read data: %s, print files info: %s, limit file number to: %s\n\n",
    $nohash ? 'NO' : 'YES',
    $nodata ? 'NO' : 'YES',
    $printfiles ? 'YES' : 'NO',
    $maxfiles == 0x7fffffff ? 'NO' : $maxfiles;

die "ERROR: mph executable not found in ${mph}/mph" if ( ! (-f "${mph}/mph") );
die "ERROR: mph executable not found" if ( not (-e "${mph}/emitc") );

my %info;
my $files = make_part_list($ldraw, \%info, $nodata, $maxfiles);

if ( $nohash == 0 ) {
    make_mph_hash($mph);
}

#my @name_in_order;
#map_name_to_hash(\@name_in_order);


wr_library($libname, \%info, $files, $printfiles, $nodata);

sub wr_library {
    my $libname = shift;
	my $info = shift;
	my $files = shift;
    my $printfiles = shift;
    my $nodata = shift;
    
	my $magic = 0xbad0beef;
	
	open OF, ">${libname}" or die "ERROR: can't open library.bin";
	
	my $curr_offset;
	
	# magic
	my $packed = pack("V", $magic);
	print OF $packed;
	
	# number of sections
	my $num_of_sections = scalar(keys %$info);
	printf "Header will have %d sections\n\n", $num_of_sections;
	$packed = pack("V", $num_of_sections);
	print OF $packed;

    my @sorted_keys = sort {$info->{$a}->{offset} <=> $info->{$b}->{offset} } keys %info;
    
    printf "Section order: [%s]\n\n", join(',', @sorted_keys);
	# section headers (including section headers)
	my $header_size = 8 + $num_of_sections*8;
	for my $k (@sorted_keys) {
		printf "Section [%s] header size=%d, offset=%d ", $k, $info->{$k}->{size},
			$info->{$k}->{offset};
        my $offset_in_file = 8*$info->{$k}->{offset} + $header_size;
        printf " actual offset in file=%08x\n", $offset_in_file;
		# size of section
		$packed = pack("V", $info->{$k}->{size});
		print OF $packed;
		# pointer to start of section
		$packed = pack("V", $offset_in_file);
		print OF $packed;
	}

    printf "\nSections data starting @ %d %08x\n\n", $header_size, $header_size;

    # sections
		
	# calc size of names and data parts
    my $total_name_len = 0;
    my $total_sec_len = 0;
	for my $k (@sorted_keys) {
        my $name_len = 0;
        my $data_len = 0;
		my $base = $info->{$k}->{offset};
		for my $i (0..($info->{$k}->{size}-1)) {
            $name_len += (length($files->[$base + $i]->{name}) + 1);
            $data_len += (length($files->[$base + $i]->{data}) + 1) unless $nodata;
		}
        $total_sec_len += $info->{$k}->{size};
		$info->{$k}->{name_len} = $name_len;
		$total_name_len += $name_len;
		printf "Section '%s' have %d bytes of names %d bytes of data\n", $k, $name_len, $data_len;
	}

    # header size including sections
    $header_size += $total_sec_len*8;

	# now write sections with correct pointer to name and data storage
	# that will be written later.
    my $curr_offset_name = 0;
    my $curr_offset_data = 0;
	for my $k (@sorted_keys) {
		my $base = $info->{$k}->{offset};
		for my $i (0..($info->{$k}->{size}-1)) {
            if ( $printfiles ) {
                printf "File %d in section '%s' = [%s] ", $i, $k, $files->[$base + $i]->{name};
                printf "name pointer = %d %08x\n", $header_size + $curr_offset_name,
                    $header_size + $curr_offset_name;
            }

            # pointer to file name
			$packed = pack("V", $header_size + $curr_offset_name);
			print OF $packed;
            $curr_offset_name += (length($files->[$base + $i]->{name}) + 1);
            #printf "\tname_offset -> %d %08x\n", $curr_offset_name, $curr_offset_name;
            # pointer to data
			$packed = pack("V", $header_size + $total_name_len + $curr_offset_data);
			print OF $packed;
            if ( $nodata == 0 ) {
                $curr_offset_data += (length($files->[$base + $i]->{data}) + 1);
            }
		}
	}
	
    printf "\nFilenames starting @ %d %08x\n\n", $header_size, $header_size;
    #return;
    # names
    for my $k (@sorted_keys) {
        my $base = $info->{$k}->{offset};
        for my $i (0..($info->{$k}->{size}-1)) {
            $packed = pack("A*c", $files->[$base + $i]->{name}, 0);
            print OF $packed;
        }
    }

    $header_size += $total_name_len;
    printf "\nData starting @ %d %08x\n\n", $header_size, $header_size;
    # data
    if ( $nodata == 0 ) {
        for my $k (@sorted_keys) {
            my $base = $info->{$k}->{offset};
            for my $i (0..($info->{$k}->{size}-1)) {
                $packed = pack("A*c", $files->[$base + $i]->{data}, 0);
                print OF $packed;
            }
        }
    }

	close OF;
}

sub make_part_list {
	my $dir = shift;
	my $info = shift;
    my $nodata = shift;
	my $maxfile = shift;

    #$maxfile = 3;
    
	my @files;
	my $total_off = 0;
	
    my @primitive_dirs = ('parts', 'p', 'p/48', 'parts/s');
    for my $d (@primitive_dirs) {
        my @files_in_dir = `find ${dir}/${d} -maxdepth 1 -type f | head -${maxfile}`;
        chomp(@files_in_dir);
        for my $f (@files_in_dir) {
            my $data = `cat $f` unless $nodata;
            #
            # p/1-16con1.dat -> 1-16CON1
            # p/48/1-12chrd.dat -> 48/1-12CHRD
            # parts/s/003238s1.dat -> S/003238S1
            #
            $f =~ s/$dir[\/]*(parts)*[\/]*(p)*[\/]*//;
            
            $f =~ tr/a-z/A-Z/;
            push @files, {name => $f, data => $data};
        }
        $info->{$d} = {offset => $total_off, size => scalar(@files_in_dir)};
        $total_off = scalar(@files);
        printf "LDRAW: [%s] have %d files\n", $d, scalar(@files_in_dir);
    }
    
#    # LDraw/parts
#	my @files_part = `find ${dir}/parts -maxdepth 1 -type f | head -${maxfile}`;
#	chomp(@files_part);
#	for my $f (@files_part) {
#        my $data = `cat $f` unless $nodata;
#		$f =~ s/$dir[\/]*parts[\/]*//;
#        push @files, {name => $f, data => $data};
#	}
#	$info->{parts} = {offset => $total_off, size => scalar(@files_part)};
#	$total_off = scalar(@files);
#	printf "LDRAW: parts/ have %d\n", $#files_part;
#
#    # LDraw/p
#	my @files_p = `find ${dir}/p -maxdepth 1 -type f | head -${maxfile}`;
#	chomp(@files_p);
#	for my $f (@files_p) {
#        my $data = `cat $f` unless $nodata;
#		$f =~ s/$dir[\/]*//;
#        push @files, {name => $f, data => $data};
#	}
#	$info->{parts_p} = {offset => $total_off, size => scalar(@files_p)};
#	$total_off = scalar(@files);
#	printf "LDRAW: parts/p/ have %d\n", $#files_p;
#
#    # LDraw/p/48
#    my @files_p_48 = `find ${dir}/p/48 -maxdepth 1 -type f | head -${maxfile}`;
#    chomp(@files_p);
#    for my $f (@files_p_48) {
#        my $data = `cat $f` unless $nodata;
#        $f =~ s/$dir[\/]*//;
#        push @files, {name => $f, data => $data};
#    }
#    $info->{parts_p_48} = {offset => $total_off, size => scalar(@files_p)};
#    $total_off = scalar(@files);
#    printf "LDRAW: parts/p/48/ have %d\n", $#files_p;
#
#
#    # LDraw/parts/s
#	my @files_s = `find ${dir}/parts/s -maxdepth 1 -type f | head -${maxfile}`;
#	chomp(@files_s);
#	for my $f (@files_s) {
#        my $data = `cat $f` unless $nodata;
#		$f =~ s/$dir[\/]*parts[\/]*//;
#        push @files, {name => $f, data => $data};
#	}
#	$info->{s} = {offset => $total_off, size => scalar(@files_s)};
#	$total_off = scalar(@files);
#	printf "LDRAW: s/ have %d\n", $#files_s;

	open PARTLIST, ">ldraw.lst" or die "Can't open out";

	for my $f (@files) {
        printf PARTLIST "%s\n", $f->{name};
	}
	close PARTLIST;
	
	return \@files;
}

sub make_mph_hash {
	my $mph = shift;
	
	#
	#  ./mph-1.2/mph -d3 -l <ldraw.lst 2>/dev/null | ./mph-1.2/emitc -s -l >ldraw_parts.c
	#
	system("${mph}/mph -d3 -l <ldraw.lst 2>/dev/null | ${mph}/emitc -s -l >ldraw_parts.c");
	
	open CMAIN, ">hashtst.c" or die "ERROR: can't open hashtst.c";
	print CMAIN << "END_OF_C";
#include <stdio.h>
#include <string.h>

extern int hash(const char *, int len);

int main()
{
	char word[4096+1];
	while (gets(word)) {
		printf("%s=%d\\n", word, hash(word, strlen(word)));
	}
    return 0;
}
END_OF_C
	
	close CMAIN;
	
	system("gcc ldraw_parts.c hashtst.c");
	die "ERROR: C compilation failed" unless (-f "a.out") or (-f "a.exe");
    unlink "a.out";
}

sub map_name_to_hash {
	my $name_in_order = shift;
	my $exe;
	$exe = "./a.exe" if (-e "./a.exe");
	$exe = "./a.out" if (-e "./a.out");
	die "ERROR: no exe" unless defined ($exe);
	
	my @namehash = `cat ldraw.lst | $exe`;
	chomp(@namehash);
	printf "I have %d parts at the end\n", $#namehash;
	for my $s (@namehash) {
		my @v = split('=', $s);
		$name_in_order->[$v[1]] = $v[0];
	}
}
