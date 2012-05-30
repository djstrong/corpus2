#!/usr/bin/perl

use File::Find;

if ( @ARGV == 2 ){

$dir = $ARGV[0];
$relname = $ARGV[1];


find(\&wanted, $dir);

sub wanted{
	my $F;
	$filename = $_;
	my @file;
	if (/.*xml/){
		sysopen(F, $filename, "r");
		$isRel = 0;
		my $prevLine;
		my $nextLine;
		while(<F>){
			push(@file, $_);
			if( /name="$relname"/ ){
				$isRel = 1;
			}elsif( $isRel and /<\/rel>/){
				$isRel = $isRel + 1;
			}

			if( $isRel == 2){
				$isRel = 0;
				$last = $#file;
				$prevLine = $file[$last-2];
				$nextLine = $file[$last-1];
	
				$prevLine =~ /(chan=".*">[0-9]+<)/;
				$prevChan = $1;
				$nextLine =~ /(chan=".*">[0-9]+<)/;
				$nextChan = $1;

				$prevLine =~ s/chan=".*">[0-9]+</$nextChan/;
				$nextLine =~ s/chan=".*">[0-9]+</$prevChan/;

				$file[$last-2] = $prevLine;
				$file[$last-1] = $nextLine;
			}
		}
		close(F);
		open F, ">$filename";
		print F "@file";

	}

}

}else{

	print "USAGE: swapchans.pl directory relation_name\n";
}
