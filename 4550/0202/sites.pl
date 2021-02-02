#!/usr/bin/perl
use 5.16.0;
use warnings FATAL => 'all';
use Getopt::Std;

our ($opt_e, $opt_d, $site);
getopt('ed');

sub enable {
    say "Enable: $opt_e";
    system(qq{ln -s "/etc/nginx/sites-available/$opt_e"} .
           qq{ "/etc/nginx/sites-enabled"}); 
}

sub disable {
    say "Disable: $opt_d";
    system(qq{rm "/etc/nginx/sites-enabled/$opt_d"});
}

sub list {
    say "== Available ==";
    system("ls sites-available");
    say "== Enabled ==";
    system("ls sites-enabled");
}

enable()  if  ($opt_e);
disable() if  ($opt_d);
list() unless ($opt_d || $opt_e);
