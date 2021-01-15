#!/usr/bin/perl
use 5.20.0;
use warnings FATAL => 'all';
use autodie qw(:all);
use IO::Handle;

sub cleanup {
    my ($path) = @_;
    rename($path, "$path.tmp");
    open(my $src, "<", "$path.tmp");
    open(my $dst, ">", "$path");
    $dst->say("+++");
    $dst->say(qq{title = "$path"});
    $dst->say("+++");

  LINE:
    while (<$src>) {
        chomp;
        if (/^(---|\+\+\+)$/) {
            while (<$src>) {
                last LINE if (/^(---|\+\+\+)$/);
            }
        }
    }

    while (<$src>) {
        chomp;
        $dst->say($_);
    }

    close($dst);
    close($src);

    unlink("$path.tmp");
}


my @mds = `find . -name "*.md"`;

for my $md (@mds) {
    chomp $md;
    cleanup($md);
}
