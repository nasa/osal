#!/usr/bin/perl

#
#  NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
#
#  Copyright (c) 2020 United States Government as represented by the
#  Administrator of the National Aeronautics and Space Administration.
#  All Rights Reserved.
#
#  Licensed under the Apache License, Version 2.0 (the "License"); you may
#  not use this file except in compliance with the License. You may obtain
#  a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#

#
# Stub function generator script
# ------------------------------
#
# PURPOSE:
#   Given a set of public API C header files, this generates a matching set of
#   stub functions to be used with coverage testing.  Generating the stub functions
#   in this manner ensures the argument and return types are consistent between
#   the stub and the declaration.
#
# USAGE:
#   generate_stubs.pl <output-stub-dir> <header-file> ...
#
# The script will scan each header file given on the command line, and search them
# for C function prototypes.  Each prototype will then be used to define a matching stub
# implementation in the output stub directory.  A .c file will be created with the same
# basename as the .h file containing stub versions of the functions defined in that header.
#
# File boilerplate info (i.e. first block comment) is preserved between input and output file
# such that this should be able to work with any subsystem without altering the legal info
# in the file header.
#
# By default the stubs are rendered with a default handler following the pattern:
#    UT_DefaultHandler_${STUBNAME}
#
# By default the stubs are written to a file with the same basename and a "-stubs" suffix.
#
# This script will check corresponding handler file (via the handler-suffix option) for the presence
# of a handler.  A reference to the handler will only be generated in the stub if it exists.
# This is a convenience option to avoid rendering references to handlers that do not exist,
# while still allowing the set to grow over time.

#
# Options:
#
#   --no-handler: a NULL default handler will be rendered for all stubs
#   --all-handler: a non-NULL default handler will be rendered for all stubs
#             (note the result will fail to link unless handler is actually implemented)
#   --filter: a perl regex expression to apply on function names (e.g. /_Impl$/)
#   --stub-suffix: a suffix to use for the generated file names ("stubs" by default)
#   --handler-suffix: a suffix to use for the generated file names ("handlers" by default)
#

use strict;
use warnings;

my $publicapi = {};

# a list of functions that this should NOT generate a stub for.
# In particular these are the "real thing" even in a coverage environment
my $ignorelist = {
    OS_Application_Run => 1,
    OS_Application_Startup => 1,
    CompileTimeAssert => 1          # asserts can sometimes look like prototypes
};

my %options = (
        "stub-suffix" => 'stubs',
        "handler-suffix" => 'handlers'
);

my @hdrlist = grep(/\S/, map {
    if (s/^--//) {
        if (s/=(.*)$//) {
            $options{$_} = $1;
        } else {
            $options{$_} = 1;
        }
        $_ = "";
    }
    $_;
} @ARGV);

# currently the output dir is the first positional arg, shift it out.
# could make this an option if needed
my $stubpath = shift (@hdrlist);

unless (defined $stubpath && -d $stubpath) {
    die "Usage: $0 <output-stub-directory> <header-file> ...\n";
}

foreach my $hdr (@hdrlist)
{
    next unless open(HDR, "$hdr") || die "Cannot open: $hdr for reading\n";

    my $fileapi = {};
    my @lines = ();
    my $file = "";
    my $file_boilerplate;
    my $file_variadic;
    my @ifdef_level = (1);

    # All header files start with some legal boilerplate comments
    # Take the first one and save it, so it can be put into the output.
    while(<HDR>)
    {
        if (!$file_boilerplate && !/\S/) {
            # at first blank line, consider it the end of boilerplate code
            $file_boilerplate = join('', @lines);
            @lines = ();
        } else {
            # Normal code processing
            # if line has continuation (\) at end, remove the newline too,
            # so it will be in a single "line" in the result.
            chomp if (s/\\$//);
        }

        # detect "#ifdef" lines - some may need to be recognized.
        # at the very least, any C++-specific bits need to be skipped.
        # for now this just specifically looks for __cplusplus
        if (/^\#(if\w+)\s+(.*)$/) {
            my $check = $1;
            my $cond = $2;
            my $result = $ifdef_level[0];

            if ($cond eq "__cplusplus" && $check eq "ifdef") {
                $result = 0;
            }

            unshift(@ifdef_level, $result);
        }
        elsif (/^\#else/) {
            # invert the last preprocessor condition
            $ifdef_level[0] = $ifdef_level[0] ^ $ifdef_level[1];
        }
        elsif (/^\#endif/) {
            shift(@ifdef_level);
        }
        elsif ($ifdef_level[0]) {
            push(@lines, $_) ;
        }
    }
    close(HDR);

    foreach (@lines)
    {
        # Truncate each line at C++-style comment
        s/\/\/.*$//;
    }

    # combine all content into a single string
    # this eases processing of multi-line constructs
    $file = join('', @lines);

    # take out comments
    $file =~ s/\/\*(.*?)\*\///msg;

    # take out preprocessor lines
    $file =~ s/^\s*\#.*?$//msg;

    # take out all content inside braces {}
    # this is done in a loop to handle nested braces
    while ($file)
    {
        last unless ($file =~ s/\{[^{}]*\}/;/gsm);
    }

    # split on semicolons, and eliminate blank lines
    # now each array entry should be some sort of C declaration
    @lines = grep(/\S/, split(/;/, $file));

    # trim leading/trailing whitespace on each declaration
    s/^\s+|\s+$//g foreach (@lines);

    # Now Find function prototypes
    foreach (@lines) {
        next if (/\btypedef\b/);        # ignore typedefs
        next if (/\bstatic inline\b/);  # ignore

        # discard "extern" qualifier
        # (but other qualifiers like "const" are OK and should be preserved, as
        # it is part of return type).
        s/\bextern\b//;

        # The following macros are defined in OSAL common_types.h as function attributes
        # They may appear on other function declarations, and should be ignored here.
        s/\b_EXTENSION_\b//;
        s/\bOS_USED\b//;
        s/\bOS_PRINTF\(.*?\)//;

        # scrub whitespace for consistency - multiple spaces become single space
        # and trim leading/trailing spaces again
        s/^\s+//;
        s/\s+$//;
        s/\s+/ /g;

        # snapshot the original line
        my $rt = $_;

        # filter for lines that meet general pattern:
        #   <function-name> ( <args> );
        #
        # Note this is a regex match and does not attempt to handle all
        # possible C tokens/grammar, but because of coding standards and formatting
        # the input shouldn't be too obscure (i.e. the code should not have
        # old-style C argument lists, trigraphs, etc).
        #
        # Because return types may be qualified (i.e. not just a single token/word and
        # also may be a pointer, this focuses on only the function name and argument
        # list parts.  Function names in C are limited to alphanumeric chars w/underscore.
        next unless ($rt =~ s/([A-Za-z0-9_]+) ?\((.*?)\)$//);

        my $fn = $1;      # function name
        my $argstr = $2;  # arguments as a comma-separated string (C-style)
        my @argnames = (); # arguments names as a list (to be filled later)
        my %argtypes = (); # arguments types as a hash (to be filled later)
        my $func_variadic;

        # trim whitespace again on the components
        # note the leftover text in "rt" constitutes the return type.
        $argstr =~ s/^\s+|\s+$//g;
        $rt =~ s/^\s+|\s+$//g;

        # skip the rest if its part of the ignore list
        next if ($ignorelist->{$fn});

        # apply the filter
        if (defined $options{filter}) {
            local $_ = $fn;
            next unless eval $options{filter};
        }

        # Show it on console for debug
        print ("Identified API: $rt $fn($argstr)\n");

        # recognize "void" return type as undefined
        $rt = undef if ($rt eq "void");

        # split argument list string by commas (,) to get each argument
        # this also needs to recognize a single "void" - should result in empty list
        if ($argstr ne "void") {
            my @argtemp = split(/\s*,\s*/, $argstr);
            foreach my $arg (@argtemp) {
                my $argtype = $arg;
                if ($argtype =~ s/([A-Za-z0-9_]+)$//) {
                    push(@argnames, $1);
                    $argtypes{$1} = $argtype;
                } elsif ($argtype eq "...") { # variadic function
                    $func_variadic = 1;
                    $file_variadic = 1;
                }
            }
        }

        # save the API details in a hash, keyed by function name, as follows:
        #  decl => full declaration in its original form (mostly)
        #  rt => return type
        #  argnames => list of argument names
        #  argtypes => list of argument types
        $fileapi->{$fn} = { decl => $_, rt => $rt, argnames => \@argnames, argtypes => \%argtypes, variadic => $func_variadic };

    }

    # If file API is not empty (i.e. some prototypes were found) then add it
    # to the public API structure based on the filename
    if (%{$fileapi}) {
        my @filenameparts = split(/\//, $hdr);
        my $basename = pop(@filenameparts);

        $basename =~ s/\..*$//; # trim filename extension
        $publicapi->{$basename} = { boilerplate => $file_boilerplate, variadic => $file_variadic, api => $fileapi };
    }
}

# Now generate a stub function for all items in API
# This creates a source file per header file, with a stubs suffix.
foreach my $basename (sort keys %{$publicapi}) {

    my $fileapi = $publicapi->{$basename}->{api};
    my $boilerplate = $publicapi->{$basename}->{boilerplate};
    my @functions = sort keys %{$fileapi};

    # file names are not consistent WRT use of - or _ chars.
    # if file name uses -, then use -, otherwise use _.
    my $refname = "$stubpath/$basename";
    if ($basename =~ /-/) {
        $refname .= '-';
    } else {
        $refname .= '_';
    }
    my $stubfile = $refname . $options{"stub-suffix"} . '.c';
    my $handlerfile = $refname . $options{"handler-suffix"} . '.c';

    my $handler_func = {};
    my $handler_prefix = "UT_DefaultHandler_";

    if ($options{"all-handler"}) {
        foreach my $funcname (@functions) {
            $handler_func->{$funcname} = $handler_prefix.$funcname;
        }
    } elsif (!$options{"no-handler"}) {
        if (open(HANDLER, $handlerfile)) {
            while (<HANDLER>) {
                if (/void\s+$handler_prefix(\S+)\s*\(/) {
                    $handler_func->{$1} = $handler_prefix.$1;
                }
            }
            close(HANDLER);
        }
    }

    # Now actually write the output stub source file
    # NOTE: no need to be too fussy about whitespace and formatting here
    # as the output file will be passed to clang-format at the end.
    open(OUT, ">$stubfile") || die "Cannot open $stubfile for writing";

    print OUT $boilerplate . "\n";
    print OUT "/**\n";
    print OUT " * \@file\n";
    print OUT " *\n";
    print OUT " * Auto-Generated stub implementations for functions defined in $basename header\n";
    print OUT " */\n\n";

    # if the file uses any variadic functions, then stdarg.h will be needed.
    if ($publicapi->{$basename}->{variadic}) {
        print OUT "#include <stdarg.h>\n\n";
    }

    print OUT "#include \"$basename.h\"\n";
    print OUT "#include \"utgenstub.h\"\n\n";

    unless ($options{"no-handler"}) {
        foreach my $funcname (@functions) {

            next unless ($handler_func->{$funcname});

            my $args = "void *, UT_EntryKey_t, const UT_StubContext_t *";
            if ($fileapi->{$funcname}->{variadic}) {
                $args .= ", va_list";
            }
            print OUT "void ".$handler_func->{$funcname}."($args);\n";
        }
    }

    print OUT "\n";

    foreach my $funcname (@functions) {

        print OUT "/*\n";
        print OUT " * ----------------------------------------------------\n";
        print OUT " * Generated stub function for $funcname()\n";
        print OUT " * ----------------------------------------------------\n";
        print OUT " */\n";

        # create local vars for ease of access
        my $rt = $fileapi->{$funcname}->{rt};
        my $decl = $fileapi->{$funcname}->{decl};
        my $variadic = $fileapi->{$funcname}->{variadic};
        my @argnames = @{$fileapi->{$funcname}->{argnames}};
        my %argtypes = %{$fileapi->{$funcname}->{argtypes}};
        my @handler_args = ();

        print OUT "$decl\n";
        print OUT "{\n";

        if ($variadic) {
            print OUT "va_list UtStub_ArgList;\n\n";
        }

        if ($rt) {
            print OUT "UT_GenStub_SetupReturnBuffer($funcname, $rt);\n\n";
        }

        foreach my $arg (@argnames) {
            print OUT "UT_GenStub_AddParam($funcname, $argtypes{$arg}, $arg);\n";
        }

        print OUT "\n";

        push(@handler_args, $funcname);

        if ($variadic) {
            push(@handler_args, "Va");
        } else {
            push(@handler_args, "Basic");
        }

        if ($handler_func->{$funcname}) {
            push(@handler_args, $handler_func->{$funcname});
        } else {
            push(@handler_args, 'NULL');
        }

        # Variadic stub functions have a different handler that includes a va_list
        if ($variadic) {
            push(@handler_args, "UtStub_ArgList");
            print OUT "va_start(UtStub_ArgList, $argnames[$#argnames]);\n";
        }

        print OUT "UT_GenStub_Execute(" . join(', ',@handler_args) . ");\n";

        if ($variadic) {
            print OUT "va_end(UtStub_ArgList);\n";
        }

        print OUT "\n";

        if ($rt) {
            print OUT "return UT_GenStub_GetReturnValue($funcname, $rt);\n";
        }

        print OUT "}\n\n";
    }

    close(OUT);

    # invoke clang-format on the output, so it will have consistent whitespace
    system("clang-format -style=file -i \'$stubfile\'") == 0 || die "Unable to call clang-format\n";

    print "Generated $stubfile\n";
}
