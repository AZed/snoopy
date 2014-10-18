# Snoopy Logger #



## 1. DESCRIPTION ##

Snoopy is designed to aid system administrators by providing a log of commands
that were executed on the system. Snoopy should be completely transparent to
users and to applications. It acts as a preloaded library that provides a
wrapper around calls to execve() syscall. Logging is done via syslog.



## 2. INSTALLATION ##

Snoopy supports various features that  can be enabled by supplying arguments
to configure command. Consult `./configure --help' for more information.


### Download options:

You can download snoopy release tarballs from this location:
http://source.a2o.si/download/snoopy/

Alternatively you can download it from GitHub, by selecting a release tag at
the branch dropdown selector, and then clicking the ↓ZIP button, next to the
repository URL, below project description.
(I know this is awkward, but GitHub has discontinued the "Uploads" feature)


### Installation procedure is simple:

    # Only if you are building directly from git repository:
    ./autogen.sh

    # Check configuration options:
    ./configure --help

    # Then continue with normal build procedure:
    ./configure [OPTIONS]
    make
    make install

At this point, snoopy is **installed but not yet enabled**.


### Configuring log output

Snoopy already has default log message format configured, but by using
"./configure --with-message-format=FORMAT" you can adjust it to your
needs.

Log message format specification example:

    --with-message-format="text1:%{input1} text2:%{input2} text3:%{input3:arg}"

Text outside %{...} is considered literal and is copied as-is to final
log message. On the other hand, text found within %{...} has special
meaning: it calls input provider. If input provider specification
contains a colon, then text before colon is considered input provider
name, and text following the colon is passed as argument to the provider
in question.


### Configuring filtering

Snoopy supports message filtering. Filtering must be configured at
build time, here is an example:

    # REQUIRED TO ENABLE FILTERING FEATURE
    --enable-filter

    # HOW TO DEFINE FILTERS
    --with-filter-chain="exclude_uid:0"       # Log all commands, except the ones executed by root
    --with-filter-chain="exclude_uid:1,2,3"   # Log all commands, except those executed by users with UIDs 1, 2 and 3
    --with-filter-chain="only_uid:0"          # Log only root commands
    --with-filter-chain="filter1:arg11;filter2:arg21,arg22;filter3:arg31,32,33"

Here you have four filter definitions for your reference, they are
quite self-explanatory. As you probably noted in the last example,
multiple filters may be defined in a chain, separated by semicolon.

Each filter chains can contain multiple filter definitions. They are
processed in order of appearance. If any of the filters decides the
message should be dropped, the filter chain processing is immediately
interrupted and message is not passed to syslog.

If filter requires an argument, they may be passed to them by
specifying a colon after filter name, followed by an argument.
Argument is passed to the filter as-is. If passing of multiple
arguments to filter is required, they are passed as single string
and must be parsed/tokenized by the filter itself (see "only_uid"
filter for example).

Filter chain specification may not contain any spaces.
(Acutally spaces are allowed in arguments, but not in filter names
and between semicolons and filter names, nor between filter names
and following colons.)





## 3. HOW TO ENABLE ##

### 3.a) Enable for specific program ###

If you wish to monitor only certain applications you can do so through
the LD_PRELOAD environmental variable - simply set it to the full path
to snoopy.so shared library before starting the application.

Example:

    export LD_PRELOAD=/usr/local/lib/libsnoopy.so    # default path
    lynx http://linux.com/
    unset LD_PRELOAD


### 3.b) Enable system-wide snoopy on 32-bit-only or 64-bit-only systems ###

WARNING: Using this method on multilib systems (64-bit systems capable
WARNING: of running 32-bit applications) can cause malfunction because
WARNING: preload config file /etc/ld.so.preload makes  no  distinction
WARNING: between 32- and 64-bit programs and shared libraries.

    # Enable it using entry in /etc/ld.so.preload
    make enable

An entry is created in /etc/ld.so.preload file  which  causes  execv()
and execve() system calls to be intercepted by snoopy and logged via
syslog.


### 3.c) For multilib systems ###

Content of /etc/ld.so.preload should include the following line:

    /usr/local/$LIB/libsnoopy.so

This applies only when you have installed both 32bit and 64bit version
of the library in the appropriate paths.


### 3.d) For multilib systems with LD_PRELOAD_* environmental variables ###

On systems that support LD_PRELOAD_32 and LD_PRELOAD_64  you  can  use
those variables to force loading of snoopy. If you with to  enable  it
system-wide, ensure that correct values are held  by  those  variables
at boot time. Consult section  4.a  of  this  README  on  how  to  set
environmental variables. Setting them at boot time is usually  distro-
dependent.
Users are also required to compile 32-bit version of library. To do so
on 64-bit systems it is usually enough to set appropriate CFLAGS:

    CFLAGS=-m32 ./configure [OPTIONS]

Of course your system must be cross-compilation capable. Consult  your
OS documentation for details on this subject.



## 4. OUTPUT ##

The exact location  of  your  snoopy output  depends  on  your  syslog
configuration. Usually it gets stored in one of the following files:

    /var/log/auth*
    /var/log/messages
    /var/log/secure



## 5. HOW TO DISABLE ##

To disable snoopy, simply edit /etc/ld.so.preload and remove the
reference to snoopy.so. Also unset  any  environmental  variable  that
references snoopy (LD_PRELOAD, LD_PRELOAD_32 and LD_PRELOAD_64).  Then
you may also delete snoopy shared library from  your  system.  Default
installation path of snoopy shared library is:

    /usr/local/lib/libsnoopy.so



## 6. NOTE ##

execv() calls are now explicitly logged.   Although,  according to the
man page for execv(),  it is supposed to call execve().   To this date
the reason why execv()  calls weren't being logged is unknown,  but we
are working to find out why.



## 7. CONTRIBUTING TO SNOOPY DEVELOPMENT ##

New ideas are welcome. Most of change requests so far were about additional
log data or filtering capabilities, therefore most of development/changes
is expected in that area.

Here are basic rules for input provider development:
- input providers are located in src/input/
- input provider names should be lower case, with underscores for word separation
- data about currently executing process is available in src/inputdatastorage.*
    files. Consult existing providers on how to use it (filename for example)
- each input provider must be self-sufficient. Do not rely on other parts of snoopy
- each input provider must be tidy (free all mallocs, close all file descriptors)
- the first argument passed to each provider is a char array to return message into
- input provider message must not be longer than SNOOPY_INPUT_MESSAGE_MAX_SIZE
- each input provider must have a corresponding header file
- all input providers must build with -Wall -Werror flags (enabled by default)
- code indentation: 4 spaces, no tabs

If you have developed a shiny new input provider and you would like to
start using it with snoopy, there are three additional places where you
need to add references to it to make snoopy fully aware of it:
- src/input/Makefile.am   (location is evident)
- src/inputregistry.h     (one reference)
- src/inputregistry.c     (two references)

Rules for filter development are the same as for new input providers, with the
following additional specifics:
- filters are located in src/filters
- each filter is passed two arguments: logMessage and filter argument (if any,
    otherwise an empty string is passed)
- filter argument is literal. If it contains multiple arguments (separated by
    comma, for example), the filter itself must do the parsing/tokenization.
- filter MAY modify logMessage. If it does so, the new log message MUST NOT
    EXCEED the maximum log message size, defined in snoopy.h.
- filter MUST return SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP constant
- if SNOOPY_FILTER_DROP is returned by filter, it causes immediate termination
    of filter chain processing and message is not logged to syslog

If you have developed a shiny new filter and you would like to
start using it with snoopy, there are three additional places where you
need to add references to it to make snoopy fully aware of it:
- src/filter/Makefile.am   (location is evident)
- src/filterregistry.h     (one reference)
- src/filterregistry.c     (two references)

Pushing code upstream:
- your commits should be easily readable, with concise comments
- your commits should follow the KISS principle: do one thing, and do it well
- same goes for pull requests - one pull request should contain one change only
    (one bugfix or one feature at a time)
- if you have developed multiple features and/or bugfixes, create separate
    branches for each one of them, and request merges for each branch
- the cleaner you code/change/changeset is, the faster it will be merged

That is it. Happy coding! :)



## 8. CREDITS ##

Snoopy Logger was created by:
     marius@umich.edu
        mbm@linux.com

It is currently maintained by:
    bostjan@a2o.si

Development is located at the following URI (see pull requests for
contributor credits):
    http://github.com/a2o/snoopy/



## X. WARNING TO FEDORA/REDHAT/CENTOS USERS - OBSOLETE, FIXED IN 1.9.0 ##
(and possibly other distributions)

Due to bug described at the links below, **please make sure you test your Snoopy
installation before you deploy it to production**. The bug manifests itself at
shutdown, when system hangs instead of finishing the whole shutdown procedure.

So far this has only been reproduced on Fedora, and there is a reference to
Hyper-Threading, but no one has come up with a valid solution, only workarounds.

Bug descriptions:
https://bugzilla.redhat.com/show_bug.cgi?id=745603
https://github.com/a2o/snoopy/issues/1

**How to test**
- Configure, build, install, enable
- Make sure it is working by tailing log files
- Reboot #1
- Reboot #2. If second reboot is successful, you are on the safe side

Distributions known to work OK:
* Ubuntu Server 12.04.01
* Slackware 12.2+