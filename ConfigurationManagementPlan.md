# Introduction

This document defines the processes and standards that all
FLTK developers must follow when developing and documenting
FLTK, and how trouble reports are handled and releases are
generated. The purpose of defining formal processes and
standards is to organize and focus our development efforts,
ensure that all developers communicate and develop software with
a common vocabulary/style, and make it possible for us to
generate and release a high-quality GUI toolkit which can be
used with a high degree of confidence.

Much of this file describes the existing practices that have
been used up through FLTK 1.1.x, however I have also added some
new processes/standards to use for future code and releases.

# Communication

The [fltk.coredev](https://groups.google.com/g/fltkcoredev) google mailing list
and newsgroup are the primary means of communication between developers.

_All_ major design changes must be discussed prior to implementation.

We use [GitHub Issues](https://github.com/fltk/fltk/issues) to manage bugs.

Please see the CMP section on [Managing GitHub Issues](#ManagingGitHubIssues)
for how developers should manage GitHub issues.

It is wise for all developers to monitor these NNTP based newsgroups for bug 
and commit activity:

 * fltk.commit - ([nntp](news://fltk.org:1024/fltk.commit)/[web](https://www.fltk.org/newsgroups.php?gfltk.commit+T)): 
   All fltk developer commits on GitHub commit. Includes old pre-Oct 2018 SVN commits.
 * fltk.issues - (nntp/web) All new/current fltk bugs as GitHub Issues. See "Managing GitHub Issues"

&#x25cf; fltk.bugs   &mdash; (
nntp
/
web
) The "
Old STR Bug Management System
" activity 
(replaced by GitHub Issues)

See "
Managing Old STR's
"



To monitor these groups, see the 
[newsgroup](https://www.fltk.org/newsgroups.php)

page, and use either the web interface, or NNTP instructions.






### - - -   HISTORICAL INFORMATION   - - -



STR to GitHub Issues



In July 2020 we disabled the old STR bug system from accepting new bugs, replaced by the new
"GitHub Issues" system. 
Old bugs
can still be managed and closed with the old STR system,
see the 
[old CMP STR Bug Management documentation](cmp-old-str-management.php)
.



SVN to GIT



In October 2018 we moved the FLTK code repository from Subversion (SVN) to Git, with the master

[FLTK Git Repository on GitHub](https://github.com/fltk/fltk)
.






                                # 
                                    [Specific Goals](null)
                                

                                The specific goals of the FLTK are as follows:

                                

                                    * Develop a C++ GUI toolkit based upon sound
                                            object-oriented design principles and experience. (*)

                                    * Minimize CPU usage (fast). (*)

                                    * Minimize memory usage (light). (*)

                                    * Support multiple operating systems and windowing
                                            environments, including UNIX/Linux, macOS, Microsoft
                                            Windows, and X11, using the "native" graphics
                                            interfaces. (*)

                                    * Support OpenGL rendering in environments that
                                            provide it. (*)

                                    * Provide a graphical development environment for
                                            designing GUI interfaces, classes, and simple
                                            programs. (*)

                                    * Support UTF-8 text.

                                    * Support printer rendering in environments that
                                            provide it.

                                    * Support "schemes", "styles", "themes", "skinning",
                                            etc. to alter the appearance of widgets in the toolkit
                                            easily and efficiently. The purpose is to allow
                                            applications to tailor their appearance to the
                                            underlying OS or based upon personal/user
                                            preferences.

                                    * Support newer C++ language features, such as
                                            templating via the Standard Template Library ("STL"),
                                            and certain Standard C++ library interfaces, such as
                                            streams. However, FLTK will not depend upon such
                                            features and interfaces to minimize portability
                                            issues.

                                    * Support intelligent layout of widgets.

                                

                                
                                    Many of these goals are satisfied by FLTK 1.1.x (*), and many
                                    complex applications have been written using FLTK on a wide
                                    range of platforms and devices. <!-- Development of the remaining
                                    features is proceeding for FLTK 2.0 with a new, namespace-based
                                    API. While 2.0 offers some limited 1.x source compatibility, the
                                    changes to the underlying widget classes are significant enough
                                    to prevent full compatibility. REMOVED TO PREVENT CONFUSION 10/04/18 -erco -->
                                

                                Note: FLTK 2.0 is no longer in development. -->

                                # 
                                    [Software Development Practices](null)
                                

                                ## 
                                    [Documentation](null)
                                

                                
                                    All widgets are documented using the
                                        
                                    [Doxygen](http://www.doxygen.nl/)

                                        software; Doxygen comments are placed in the header file for the
                                        class comments and any inline methods, while non-inline methods
                                        should have their comments placed in the corresponding source
                                        file. The purpose of this separation is to place the comments
                                        near the implementation to reduce the possibility of the
                                        documentation getting out of sync with the code.
                                

                                All widgets must have a corresponding test program which
                                    exercises all widget functionality and can be used to generate
                                    image(s) for the documentation. Complex widgets must have a
                                    written tutorial, either as full text or an outline for later
                                    publication

                                Starting with FLTK 1.3 Doxygen is used for HTML and PDF documentation.

                                
                                    The final manuals of FLTK 1.0 and 1.1 can be formatted,
                                         i.e. converted to PDF, using the
                                        
                                    [HTMLDOC](https://www.msweet.org/htmldoc/)
                                     software.
                                
                                ## 
                                    [Build System](null)
                                

                                
                                    The FLTK build system uses 
                                    [CMake](https://cmake.org/)

                                    and/or 
                                    [GNU autoconf](https://www.gnu.org/software/autoconf/)

                                    to tailor the library to the local operating system. Since FLTK 1.4
                                    the primary and recommended build system is CMake but autoconf is still
                                    supported. Current and future development is focused on CMake and new
                                    build features and options may be implemented only in CMake although
                                    all essential build features must also be implemented using autoconf.
                                

                                
                                    To improve portability, makefiles must not make use of the unique features
                                    offered by 
                                    [GNU make](https://www.gnu.org/software/make/)
                                    .
                                    See the 
                                    [Makefile Standards](#MAKEFILES)
                                     section for a
                                    description of the allowed make features and makefile guidelines.
                                

                                
                                    Additional GNU build programs such as 
                                    [GNU automake](http://www.gnu.org/software/automake)
                                     and
                                    [GNU libtool](http://www.gnu.org/software/libtool)

                                    must not be used. GNU automake produces non-portable makefiles
                                    which depend on GNU-specific extensions, and GNU libtool is not
                                    portable or reliable enough for FLTK.
                                

                                
                                    Note:
                                     Starting with FLTK 1.4.0 we do no longer bundle IDE
                                        files for Microsoft Windows (Visual Studio) and macOS (Xcode).
                                        IDE environments (project files) can be generated with CMake.
                                        See README.CMake.txt for more information about using CMake.
                                

                                ## 
                                    [Packaging](null)
                                

                                
                                    Source packages are created using the 
                                    makesrcdist

                                    script in the Git repository. The script accepts one or
                                    two arguments:
                                

                                

                                      ./makesrcdist 
                                    _version_

                                      ./makesrcdist 
                                    _version_
                                     tag
                                      ./makesrcdist snapshot
                                

                                Version should be of the form "1.1.10" or "1.1.10rc2". "rc2" in
                                this case describes the second release candidate.

                                The optional argument 'tag' (verbatim) can be used to
                                create a Git "tag" to mark the release in the Git repository.

                                The special version name "snapshot" creates a snapshot of the
                                Git repository without creating a release tag in the repository.

                                This needs more info. TBD soon.

                                
                                    Binary packages are not currently distributed by the FLTK
                                    team, however the 
                                    fltk.spec
                                     and 
                                    fltk.list

                                    files may be used to create binary packages on Linux, macOS,
                                    and UNIX. The 
                                    fltk.spec
                                     file produces a binary package
                                    using the 
                                    rpm
                                     software:
                                

                                

                                        rpmbuild -ta fltk-
                                    _version_
                                    -source.tar.gz
                                

                                
                                    The 
                                    fltk.list
                                     file is generated by the
                                    configure
                                     script and produces binary packages for many
                                    platforms using the 
                                    [EPM software](https://jimjag.github.io/epm/)
                                    .
                                    The 
                                    portable-dist
                                     and 
                                    native-dist

                                    targets of the top-level makefile create portable and native
                                    packages, respectively:
                                

                                
                                    make portable-dist
                                    make native-dist
                                

                                
                                    Future releases of FLTK may include files for use with
                                    Microsoft Visual Installer to produce 
                                    .msi
                                     files for
                                    installation on Microsoft Windows
                                    &reg;
                                    .
                                

                                ## 
                                    [FLTK Release Process](null)
                                

                                FLTK releases are created on a Linux system with the following
                                software installed:

                                

                                    * Infozip 2.3 or higher to create .zip archives

                                    * GNU tar 1.12 or higher to create .tar archives

                                    * GNU zip 1.2.4 or higher to create .tar.gz archives

                                    * Bzip2 1.0.2 or higher to create .tar.bz2 archives

                                    * Git 2.x or higher to create the release tag

                                

                                To test the release files, three systems will be required (or a single
                                Intel Mac running macOS, Windows, and Linux):

                                

                                    * A PC running a recent Linux distribution with GCC,
                                            RPM, and EPM installed

                                    * A Mac running macOS 10.4 or higher with CMake, Xcode and
                                            EPM installed

                                    * A PC running Microsoft Windows 2000 or higher with CMake
                                            and a recent version of Microsoft Visual C++ installed

                                

                                ### 
                                    [Creating the Release Archives](null)
                                

                                Each software release provides three archives:

                                

                                    * 
                                        fltk-
                                        _version_
                                        -source.tar.gz  (3.8MB as of 1.3.0)
                                    

                                    * 
                                        fltk-
                                        _version_
                                        -docs-pdf.tar.gz  (3.3MB as of 1.3.0)
                                    

                                    * 
                                        fltk-
                                        _version_
                                        -docs-html.tar.gz  (3.4MB as of 1.3.0)
                                    

                                

                                The archives contain the source and build files, the documentation
                                in a single PDF file, and the same documentation in many HTML files. The
                                following steps are performed to create the release archives:

                                

                                    1. 
                                        Change directories to a current working copy for the
                                                FLTK development branch you are releasing, e.g. "
                                        <kbd>cd
                                                fltk-1.3</kbd>
                                        ".
                                    

                                    1. 
                                        Run 
                                        <kbd>autoconf</kbd>
                                         to generate the 
                                        <kbd>configure</kbd>

                                                script.
                                    

                                    1. 
                                        Run 
                                        <kbd>./configure</kbd>
                                         to generate makefiles.
                                    

                                    1. 
                                        In the 
                                        <kbd>documentation</kbd>
                                         directory, run 
                                        <kbd>make dist</kbd>
                                        .

                                                
                                    1. 
                                        Back in the fltk directory, run the 
                                        <kbd>makesrcdist</kbd>
                                         script with
                                                the release version number, e.g. "
                                        <kbd>./makesrcdist 1.3.0</kbd>
                                        ".
                                                makesrcdist supports two commandline arguments. For more information
                                                on the second argument please see the documentation in makesrcdist.
                                    

                                    1. 
                                        Copy the archives that have been created in the
                                                
                                        <var>/tmp</var>
                                         directory to a more permanent location.
                                    

                                

                                ### 
                                    [Testing the Archives on Linux](null)
                                

                                Run the following commands to test the release:

                                

                                    rpmbuild -ta fltk-
                                    _version_
                                    -source.tar.bz2
                                    tar xvzf fltk-
                                    _version_
                                    -source.tar.gz
                                    cd fltk-
                                    _version_

                                    ./configure
                                    make all portable-dist native-dist
                                    cd test
                                    ./demo
                                

                                ### 
                                    [Testing the Archives on macOS](null)
                                

                                Run the following commands to test the release:

                                

                                    tar xvfz fltk-
                                    _version_
                                    -source.tar.gz
                                    cd fltk-
                                    _version_

                                    ./configure
                                    make all portable-dist native-dist
                                    cd test
                                    ./demo
                                

                                ### 
                                    [Testing the Archives on Microsoft Windows](null)
                                

                                
                                    Extract the files from the .tar.gz archive and then open the
                                    ide/VisualC2010/fltk.sln
                                     file in Visual C. Build the
                                    demo
                                     target in both release and debug modes to confirm
                                    that the software compiles, and then run the 
                                    demo

                                    target to test that each of the demo programs is functioning
                                    properly.
                                

                                Note: the previous paragraph is true only up to FLTK 1.3.x. Since
                                    FLTK 1.4.0 the IDE environment to be used must be generated with CMake.
                                    Starting with Visual Studio 2017 (Community) CMake is bundled with
                                    Visual Studio.

                                ### 
                                    [Uploading the Files to the Web Server](null)
                                


                                Uploading distribution files with the web browser is no longer supported.
                                Instructions how to upload distribution files need to be written.

                                <!--
                                    **************************************************************
                                    ** *FIXME* Uploading files instructions commented out.
                                    **************************************************************
                                
                                       1. Go to the following URL to upload the release archives:</p>
                                <pre class='command'>
                                <a href='/site/upload.php'>$PROJECT_URL/site/upload.php</a>
                                </pre>

                                       1. Upload each file and then verify that you can download the archives
                                           from the <a href='/software.php'>downloads page</a>.
                                           You will probably see your new files at the /bottom/ of the list.
                                           (You'll fix this in subsequent steps below)
                                       ";
                                       <p>
                                       1. To change the order of the uploaded files as they appear
                                           on the download page, or to remove outdated files, you need
                                           to ssh into the webserver and hand edit the data/software.md5 file,
                                           and change the order of the files so your new entries are at the top.
                                           <p>
                                           The main fltk.org page should now show your new release in the
                                           "Quick Info".
                                       
                                       <p>
                                       1. You may want to post an article announcing the new release.
                                           If you do this, you'll also want to change the "Stable Release News"
                                           in the index.php under "Quick Info".
                                       
                                       <p>
                                       1. You should post the new release to the fltk.announce newsgroup.
                                           To do this:<p>
                                           <ol type="a">
                                                1. Go to the web site maintenance page at
                                                <kbd><a href='/site/index.php'>$PROJECT_URL/site/index.php</a></kbd>.";
                                                You must be logged into the web page and have proper permissions.
                                                1. Click the <kbd>on</kbd> link below "Posting to 'fltk.announce' ..."
                                                to enable posting to fltk.announce
                                                1.  Using NNTP, post your article to fltk.announce
                                                1. Click the <kbd>off</kbd> link below "Posting to 'fltk.announce' ..."
                                                to disable posting to fltk.announce again.
                                           
                                        
                                        
                                        Update the website doxygen documentation for your new release;
                                            see [Uploading Documentation To Web Server](#FRP_UPLOADING_DOCS_TO_WEB).
                                        

                                    **************************************************************
                                    ** *FIXME* Uploading files instructions commented out.
                                    **************************************************************
                                -->

                                <!--
                                        Go to the web site maintenance page at
                                        [https://www.fltk.org/site/index.php](https://www.fltk.org/site/index.php).
                                        You must be logged into the web page and have proper permissions.

                                        login Edit software.md5
                                        Click _Commit Database Changes..._ to get the new download
                                        database from the live page into the main repository

                                        Update your local web page repository copy and edit the file
                                        data/software.md5 to your needs. Commit to svn.

                                        Go back to the web site maintenance page and click _Update the
                                        web site..._. Verify the Downloads page. Done.
                                
                                -->

                                ### 
                                    [Uploading Documentation To Web Server](null)
                                

                                
                                    Note:
                                     You need 'ssh' access to the web server to upload
                                    documentation. The upload process is not documented here.


                                # 
                                    [Managing Bugs with GitHub Issues](null)
                                

                                GitHub issues are submitted every time a user or vendor experiences
                                a problem with or wants a new feature in the FLTK software. "Issues" are
                                maintained on GitHub in the following states: 

                                <ul type=disc>
                                    * 
                                        <b>bug</b>
                                         &mdash; Something isn't working
                                    
                                    * 
                                        <b>enhancement</b>
                                         &mdash; New feature or request
                                    
                                    * 
                                        <b>duplicate</b>
                                         &mdash; This issue/pull request already exists
                                    
                                    * 
                                        <b>help wanted</b>
                                         &mdash; Extra attention is needed
                                    
                                    * 
                                        <b>invalid</b>
                                         &mdash; This doesn't seem right
                                    
                                    * 
                                        <b>fixed</b>
                                         &mdash; The issue or PR was fixed
                                    
                                    * 
                                        <b>wontfix</b>
                                         &mdash; The will not be worked on
                                    
                                    * 
                                        <b>no user support</b>
                                         &mdash; Issues are not for user support questions
                                    
                                
                                
                                    
                                        TBD: Add "priorities" for bugs, so important bugs can hold up releases. 
                                        Refer to old STR management
                                        .
                                    
                                
                                Normal operating procedure for an issue:
                                
                                    1.  A new issue is opened by a user or developer
                                    1.  A developer assigns issue to themself, and adjusts the issue's state as needed
                                    1.  As fixes are made, reference commits in comments that solve the issue, and ask the OP to confirm the solution
                                    1.  On confirmation from OP, close the issue, optionally referring to the final commit hash
                                    1.  Reopen the issue if there continues to be problems with the fix
                                

                                
                                    New issues are also posted to the 
                                    fltk.issues
                                     newsgroup.
                                    Multiple developers can join in an issue's thread. If an issue's discussion
                                    starts getting elaborate, suggest moving the discussion to fltk.coredev until
                                    a consensus is reached, and reference the discussion in the issue by subject and date.
                                

                                    If a general usage question is posted as an issue, any developer
                                    may immediately close with the "no user support" label, and direct
                                    users to the 
                                    fltk.general
                                     newsgroup.
                                    Note: 'GitHub Discussions' might be a more graceful way to handle this,
                                    but currently we don't use github discussions.
                                
                                During discussion, developers may propose possible resolutions and vote to
                                approve them. Each developer posts one vote of +1 (approve), -1 (veto),
                                or 0 (abstain). At least three developers must vote on the proposal, and
                                the total of all votes must be greater than 0. Once consensus is reached,
                                the issue is assigned to a developer that volunteers to resolve it.
                                
                                The assigned developer summarizes the proposed changes in the "issue"
                                and makes the required changes, either committing to master or a branch,
                                referencing commit hash#s as needed.  The developer then notifies the
                                submitter that the change has been applied and closes the issue when the
                                resolution is confirmed by the submitter or after two weeks, whichever
                                comes first.
                                
                                If the proposed changes do not resolve the problem, the developer may
                                unassign the issue to continue discussions on the corresponding forum or
                                privately discuss additional modifications with the submitter in order
                                to resolve.  When closing the issue, the developer should reference the
                                commit hash that finally solved the issue.

                                
                                    ## 
                                        <a name="GitHubIssueAssigning">Issue Management: Assigning An Issue To Yourself</a>
                                    
                                    <p>
                                      When a developer decides to fix a particular issue on GitHub, the first step
                                      is to assign it to yourself, so that other developers don't also try to work
                                      on it at the same time. If you decide you can no longer work on the issue,
                                      unassign yourself from it and add a note accordingly.
                                      
                                    <p>
                                        <I>TBD: Insert specific instructions here</I>
                                    <p>

                                          When attaching patches, use 
                                        <kbd>'diff -Naur'</kbd>
                                         or 
                                        <kbd>'git diff'</kbd>
                                        ,
                                          the latter preferred, as it includes FLTK version numbers, useful if years
                                          later someone tries to apply your patch. Try to include version#'s
                                          in patch filenames (e.g. foo_v1.patch, foo_v2.patch..) so followup patches
                                          are clear.
                                          
                                    <p>
                                      If a large fix is needed, create a branch, apply changes to the branch,
                                      and merge on completion, and delete the branch when done.
                                      
                                    <p>
                                        <I>TBD: Git example commands to commit, push, create/remove branch?
                                             Or refer to separate git fltk usage document describing developer issue resolution?</I>
                                    <p>
                                    ## 
                                        <a name="GitHubIssueFixing">Issue Management: Fixing An Issue</a>
                                    

                                    <p>
                                        When you've applied a fix to Git, be sure to update the issue:
                                        <br>
                                    <ol>
                                        * 
                                             Commit the fix, and reference the issue#, e.g.
                                            <ul>
                                                <br>
                                                <tt>git commit -m "Add '-d' debug flag to fluid (#92)"</tt>
                                            
                                        
                                        

                                        
                                             Reference the commit/branch in the issue thread, and request OP confirms fix, e.g.
                                            
                                                <br>
                                                <tt>
                                                    I committed to a branch called "issue92" in fltk 1.4.x, see commit 
                                                    <U>db07cea</U>
                                                    .
                                                                   Please test and confirm.
                                                </tt>
                                            
                                            

                                                       Note that GitHub automatically mentions the commit in the issue or PR
                                                       if you add the issue or PR number (#92) in your commit message.
                                                  
                                        
                                        

                                        
                                             Close the issue, and cleanup repo of any old branches, e.g.
                                            
                                                <br>
                                                <tt>
                                                    Fixed in commit 
                                                    <U>db07cea</U>
                                                </tt>
                                            
                                        
                                        
                                    


                                      For more information on how to format Git commit messages please see
                                      
                                    [below](#FM_CONFIGURATION_MANAGEMENT)
                                    .
                                    
                                    
                                    TBD: Albrecht to elaborate, possibly in a separate FLTK developer document on git use when fixing bugs.
                                         including pull requests, things to avoid when committing, branch management, recommended commit lineage
                                         and merging techniques, etc.
                                

                                # 
                                    [Software Releases](null)
                                

                                ## 
                                    [Version Numbering](null)
                                

                                FLTK uses a three-part version number separated by periods to
                                represent the major, minor, and patch release numbers:

                                
                                    MAJOR
                                    .
                                    MINOR
                                    .
                                    PATCH

                                        2.0.0
                                        2.0.1
                                        2.0.2
                                        2.1.0
                                

                                Beta-test releases are identified by appending the letter B
                                to the major and minor release numbers followed by the build
                                number:

                                
                                    MAJOR
                                    .
                                    MINOR
                                    'b'
                                    BUILD

                                        2.0b1
                                        2.0b2
                                        2.1b1
                                

                                Release candidates are indentified by appending the letters
                                "rc" to the major and minor release numbers followed by the build
                                number:

                                
                                    MAJOR
                                    .
                                    MINOR
                                    'rc'
                                    BUILD

                                        2.0rc1
                                        2.0rc2
                                        2.1rc1
                                

                                
                                    Git 
                                    _tags_
                                     are created for every release using the
                                    prefix '
                                    release-
                                    ' and the version number:

                                
                                    release-1.3.0b1
                                    release-1.3.0rc1
                                    release-1.3.0
                                

                                
                                    Git 
                                    _branches_
                                     are created for every complete major or
                                    minor release with the prefix '
                                    branch-
                                    ':

                                
                                    branch-1.0
                                    branch-1.1
                                    branch-1.3
                                    branch-x.y
                                

                                
                                    New development then continues in the 
                                    master

                                    branch with fixes backported to the corresponding
                                    branches
                                     as needed.

                                Each change that corrects a fault in a software sub-system
                                increments the patch release number. If a change affects the
                                overall software design of FLTK then the minor release number
                                will be incremented and the patch release number reset to 0. If
                                FLTK is completely redesigned the major release number will be
                                incremented and the minor and patch release numbers reset to 0:

                                
                                    2.0b1    First beta of 2.0
                                    2.0rc1   First release candidate of 2.0
                                    2.0rc2   Second release candidate of 2.0
                                    2.0.0    Production release of 2.0
                                    2.0.1    First patch release of 2.0
                                    2.0.2    Second patch release of 2.0
                                    2.1b1    First beta of 2.1
                                    2.1rc1   First release candidate of 2.1
                                    2.1rc2   Second release candidate of 2.1
                                    2.1.0    Production release of 2.1
                                    3.0b1    First beta of 3.0
                                    3.0rc1   First release candidate of 3.0
                                    3.0rc2   Second release candidate of 3.0
                                    3.0.0    Production release of 3.0
                                    3.0.1    First patch release of 3.0
                                

                                ## 
                                    [Generation](null)
                                

                                Software releases shall be generated for each successfully
                                completed software trouble report. All object and executable
                                files shall be deleted prior to performing a full build to
                                ensure that source files are recompiled.

                                ## 
                                    [Testing](null)
                                

                                
                                    Software testing shall be conducted according to the FLTK
                                    Software Test Plan (
                                    _Editor's note: to be written, along with
                                    an automated/semi-automated test framework_
                                    ). Failed tests
                                    cause Issues/STRs to be generated to correct the problems found.
                                

                                ## 
                                    [Releases](null)
                                

                                
                                    When testing has been completed successfully a new distribution
                                    is created and the release is tagged as specified previously.

                                    No release shall contain software that has not passed the
                                    appropriate software tests.


                                    Four types of releases are used:
                                
                                    * beta (b)
                                    * release candidate (rc)
                                    * production
                                    * patch
                                

                                and are released using the following basic schedule:
                                
                                
                                    
                                        
                                            Week
                                            Version
                                            Description
                                        
                                        
                                            T-6 weeks
                                            2.0b1
                                            First beta
                                        
                                        
                                            T-5 weeks
                                            2.0b2
                                            Second beta
                                        
                                        
                                            T-4 weeks
                                            2.0b3
                                            Third beta
                                        
                                        
                                            T-3 weeks
                                            2.0rc1
                                            First release candidate
                                        
                                        
                                            T-2 weeks
                                            2.0rc2
                                            Second release candidate
                                        
                                        
                                            T-0 weeks
                                            2.0.0
                                            Production
                                        
                                        
                                            T+N weeks
                                            2.0.1
                                            First patch
                                        
                                        
                                            T+N weeks
                                            2.0.2
                                            Second patch
                                        
                                    
                                

                                Beta releases are typically used prior to new major and minor
                                version releases. At least one release candidate is generated
                                prior to each production release.

                                
                                    ### 
                                        <a name="SR_BETA_RELEASES">Beta Releases</a>
                                    

                                    <p>Beta releases are generated when substantial changes have
                                        been made that may affect the reliability of the software. Beta
                                        releases may cause loss of data, functionality, or services and
                                        are provided for testing by qualified individuals.</p>

                                    <p>Beta releases are an OPTIONAL part of the release process and
                                        are generated as deemed appropriate by the release coordinator.
                                        Functional changes may be included in subsequent beta releases
                                        until the first release candidate.</p>

                                    ### 
                                        <a name="SR_RELEASE_CANDIDATES">Release Candidates</a>
                                    

                                    <p>Release candidates are generated at least two weeks prior to
                                        a production release. Release candidates are targeted for
                                        end-users that wish to test new functionality or bug fixes prior
                                        to the production release. While release candidates are intended
                                        to be substantially bug-free, they may still contain defects
                                        and/or not compile on specific platforms.

                                        
                                    <p>At least one release candidate is REQUIRED prior to any
                                        production release. The distribution of a release candidate
                                        marks the end of any functional improvements. Release candidates
                                        are generated at weekly intervals until all level 4/5 trouble
                                        reports are resolved.

                                        
                                    ### 
                                        <a name="SR_PRODUCTION_RELEASES">Production Releases</a>
                                    

                                    <p>Production releases are generated after a successful release
                                        candidate and represent a stable release of the software
                                        suitable for all users.</p>

                                    ### 
                                        <a name="SR_MAJOR_RELEASES">Major Releases (Version #.x.x)</a>
                                    
                                    <p>
                                        Major Releases occur when there's a major rewrite of the code, or a significant
                                        redefinition of the API.</p>

                                    ### 
                                        <a name="SR_MINOR_RELEASES">Minor Releases (Version x.#.x)</a>
                                    

                                        Minor Releases add ABI breaking features/fixes. Rarely breaks the API in any
                                        significant way.
                                        
                                    <p>

                                            Any 
                                        <a href="#FL_ABI_VERSION">FL_ABI_VERSION</a>
                                         code should be resolved
                                            in Minor Releases. Any code that looks like:
                                            
                                    <pre class='command'>
                                            #if FL_ABI_VERSION &gt;= 10401
                                            ... new ABI breaking code ...
                                            #else
                                            ... old non-ABI breaking code ...
                                            #endif
                                        </pre>

                                        ..is 'resolved' by 
                                    _removing_
                                     the 
                                    <kbd>#if/#else/#endif</kbd>
                                     and
                                        
                                    <kbd>'old non-ABI breaking code'</kbd>
                                    ,
                                        so that only the 
                                    <kbd>'new ABI breaking code'</kbd>
                                     is left in its
                                        place for default builds.
                                        
                                    <p>
                                    ### 
                                        <a name="SR_PATCH_RELEASES">Patch Releases (Version x.x.#)</a>
                                    

                                    <p>
                                        Patch releases are generated to fix priority 2-5 STRs. Patch
                                            releases may not add additional functionality from priority 1 STRs.
                                            
                                        <I>TBD: This paragraph needs updating with GitHub Issues in mind</I>
                                    <p>
                                        Patch Releases fix small problems that don't break the ABI,
                                        and must be API backwards compatible.</p>
                                

                                ABI breaking fixes/features can be added using
                                [FL_ABI_VERSION](#FL_ABI_VERSION)
                                 to #ifdef out
                                the code from default builds, but can be optionally enabled
                                by end users who need it for testing or for static builds.
                                This can be done by using 'configure --with-abiversion=1xxyy' or
                                CMake with its OPTION_ABI_VERSION to set this variable to the
                                right ABI version number and re-building FLTK and their apps.
                                For more information see README.abi-version.txt.

                                # 
                                    [File Management](null)
                                

                                This section describes how project files and directories are
                                named and managed.

                                ## 
                                    [Configuration Management](null)
                                

                                Source files shall be placed under the control of the Git software.
                                Source files shall be &quot;checked in&quot; with each change so that
                                modifications can be tracked, and each check in must reference
                                the applicable GitHub Issue affected, if any.

                                
                                    The following format 
                                    must
                                     be used for commit log messages:
                                

                                
                                    The first line must be a summary of the change (e.g. "fix OpenGL
                                    double-buffer bug") along with corresponding GitHub issue or PR number
                                    (e.g. "#92") or STR number (STR 1234). The '#' sign is reserved for
                                    GitHub issues or PR's and 
                                    must not
                                     be used for STR numbers (see
                                    explanation below).
                                

                                It is important to write a good but short description (summary) of the
                                changes in the first line. This is the only information people (users
                                and devs including yourself later) may see when searching for past changes
                                or a particular bug fix.
                                

                                    Note that referring to an issue, PR, or STR is 
                                    not sufficient
                                     because
                                    users or developers would need to open the GitHub issue or PR (or STR) to
                                    see what the fix was supposed to do.
                                


                                One of the goals we should be able to achieve is that we can create our
                                changelog (CHANGES.txt) directly from Git using the summaries of the
                                Git commit messages. This is already done in the automatically generated
                                snapshot articles which use '
                                git shortlog
                                ' to create the changelog.

                                If you are in doubt how it would look you may execute '
                                git shortlog -20
                                '
                                to view the latest 20 commit messages and/or see the latest weekly snapshot
                                article which is always reachable with one click from the
                                [FLTK home page](https://www.fltk.org/)
                                .

                                
                                Bad example:
                                
                                  Fixes issue #123
                                

                                
                                Better example:

                                
                                  Fix OpenGL double-buffer bug (#92)

                                  foo.cxx:
                                    - Detailed list of changes, by function
                                    - Include summary of design changes ("added new foo struct")

                                  bar.h:
                                    - More detailed changes
                                


                                The usage of Git and GitHub introduces some formatting 
                                requirements

                                for the best experience with all Git tools, including the creation of
                                the weekly snapshot log message:
                                
                                    * first line (summary) in present tense, imperative form
                                    * first line must not be longer than ca. 50 (max. 60) characters
                                    * first line shall not be terminated with a period ('.')
                                    * second line must be empty
                                    * all following lines must not be longer than ca. 70 characters
                                    * do not use the hash '#' character except for reasons mentioned below
                                

                                GitHub allows to refer to GitHub "Issues" and "Pull Requests (PR's)" by
                                using the '#' character and a number. Numbers of issues and PR's are
                                distinct, i.e. the number is sufficient, for instance there is PR #54
                                and Issue #67 but not vice versa. It is important to use this combination
                                '#nnn' exclusively to refer to Issues and PR's to avoid wrong interpretation
                                by GitHub.

                                
                                    You can "mention" an Issue or PR by using its number '#nnn' in a 
                                    commit
                                    message
                                     (!) or in the text of another Issue or PR and GitHub will add
                                    a note to the mentioned Issue or PR. Referring to "comment #5" of an STR
                                    would create such a (false positive) referral note in Issue or PR #5.
                                

                                You can even close an Issue or PR by adding a note to a commit message like:
                                
                                  Closes #234
                                

                                This should be added to the end of a commit message. There are more such
                                messages defined by GitHub: more docs TBD.

                                
                                    Documentation on the Git software is 
                                    [available on-line](git.php)
                                    .
                                

                                ## 
                                    [Directory Structure](null)
                                

                                Each source file shall be placed a sub-directory
                                corresponding to the software sub-system it belongs to
                                (&quot;fltk&quot;, &quot;OpenGL&quot;, etc.) To remain
                                compatible with case-insensitive filesystems, no two directory
                                names shall differ only by the case of the letters in the name.

                                ## 
                                    [Source Files](null)
                                

                                
                                    Source files shall be documented and formatted as described
                                    in the 
                                    [Coding Standards](#CODING_STANDARDS)
                                     section.
                                    To remain compatible with case-insensitive filesystems, no two
                                    filenames shall differ only by the case of the letters in the
                                    name.

                                C source files shall have an extension of ".c". C++ source
                                files shall have an extension of ".cxx". Header files shall have
                                an extension of ".h" unless used for FLTK 1.x compatibility.
                                FLTK 1.x compatibility headers shall have an extension of ".H".

                                
                                    
                                        
                                            
                                                Why use the ".cxx" extension?

                                                C++ source files can have any of the following extensions on
                                                various platforms: ".C", ".cc", ".cpp", ".cxx". Only the ".cxx"
                                                extension is universally recognized by C++ compilers as a C++
                                                source file - ".C" is not usable on macOS and Windows, ".cc"
                                                is not usable on Windows, and ".cpp" is historically considered
                                                C preprocessor output on UNIX.

                                                Since not all make programs handle C++ source files with the
                                                ".cxx" extension, the FLTK build system explicitly defines
                                                makefile rules for compiling C++ source files with an extension
                                                of ".cxx".
                                            
                                        
                                    
                                

                                IDE/compiler support source files (project files, workspaces,
                                makefiles, etc.) shall have extensions as required by the
                                IDE/compiler tool.

                                ## 
                                    [Header Files](null)
                                

                                In addition to the source file requirements, all header files
                                must utilitize so-called "guard" definitions to prevent multiple
                                inclusion. The guard definitions are named using the full path
                                in the FLTK source tree, e.g.:

                                

                                    * 
                                        <var>fltk/fltk.h</var>
                                         becomes 
                                        <kbd>_fltk_fltk_h_</kbd>
                                    

                                    * 
                                        <var>fluid/foo.h</var>
                                         becomes 
                                        <kbd>_fluid_foo_h_</kbd>
                                    

                                    * 
                                        <var>FL/Fl.H</var>
                                         becomes 
                                        <kbd>_FL_Fl_H_</kbd>
                                    

                                

                                Any non-alphanumeric (letters and numbers) characters are
                                replaced with the underscore (_) character, and leading and
                                trailing underscores are added to limit global namespace
                                pollution.

                                ## 
                                    [Makefiles](null)
                                

                                
                                    Makefiles shall be documented and formatted as described
                                    in the 
                                    [Makefile Standards](#MAKEFILES)
                                     section.

                                
                                    Static makefiles are named "Makefile". Makefiles created by
                                    the 
                                    autoconf
                                     software are named "Makefile.in". The
                                    common include file for all makefiles is named "makeinclude.in".


                                # 
                                    [Coding Standards](null)
                                

                                The following is a guide to the coding style that must be
                                used when adding or modifying code in FLTK. Most of this should
                                be obvious from looking at the code, but here it all is in one
                                spot.

                                ## 
                                    [General Coding Style](null)
                                

                                The FLTK code basically follows the K&amp;R coding style. While many
                                of the developers are not entirely satisfied with this coding style, no
                                one has volunteered to change all of the FLTK source code (currently about
                                54,000 lines of code!) to a new style.

                                The K&amp;R coding style can be summarized with the
                                following example code:

                                
                                  int function(int arg) {
                                    if (arg != 10) {
                                      printf("arg = %d\n", arg);
                                      return (0);
                                    } else {
                                      return 1;
                                    }
                                  }

                                  int function2(int arg) {
                                    for (int i = 0; i &lt; arg; i++) {
                                      stuff();
                                    }
                                    while (something) {
                                      stuff();
                                    }
                                    switch (arg) {
                                      case 0:
                                        stuff_here();
                                        break;
                                      case 1: {
                                        int var;
                                        stuff_here();
                                        break;
                                      }
                                      case 2:
                                        stuff();
                                        /* FALLTHROUGH */
                                      case 3: simple_stuff1(); break;
                                      case 4: simple_stuff2(); break;
                                      default:
                                        break;
                                    }
                                    return (0);
                                  }
                                

                                
                                
                                    * All curley braces must open on the same line as the enclosing
                                          statement, and close at the same level of indentation.
                                    * Each block of code must be indented 2 spaces.
                                    * 
                                        <b>Tabs are not allowed in source files</b>
                                        , please use only spaces
                                              for indenting.
                                    
                                    * A space also follows all reserved words.
                                    * 
                                        A space 
                                        _should_
                                         precede and follow all operators except
                                              prefix and postfix operators (
                                        <kbd>++i</kbd>
                                        , 
                                        <kbd>j--</kbd>
                                        , et al).
                                    
                                
                                

                                ## 
                                    [Source File Documentation](null)
                                

                                Each source file must start with the standard FLTK header containing
                                the description of the file, and FLTK copyright and license notice:

                                
                                    //
                                    // Some descriptive text for the Fast Light Tool Kit (FLTK).
                                    //
                                    // Copyright 1998-2020 by Bill Spitzak and others.
                                    //
                                    // This library is free software. Distribution and use rights are outlined in
                                    // the file "COPYING" which should have been included with this file. If this
                                    // file is missing or damaged, see the license at:
                                    //
                                    //     https://www.fltk.org/COPYING.php
                                    //
                                    // Please see the following page on how to report bugs and issues:
                                    //
                                    //     https://www.fltk.org/bugs.php
                                    //
                                

                                or the equivalent comment block using the C or other comment delimiters
                                appropriate for the source file language (configure, shell, CMake, ).

                                ## 
                                    [Documentation (Comments)](null)
                                    [*](#DR_COMMENT_STYLE)
                                

                                
                                    FLTK 1.3 and up uses Doxygen with the JavaDoc comment style to
                                    document all classes, structures, enumerations, methods, and
                                    functions. Doxygen comments are 
                                    mandatory
                                     for all FLTK
                                    header and source files, and no FLTK release will be made
                                    without complete documentation of public APIs. Here is an
                                    example of the Doxygen comment style:
                                

                                /**
                                  The Foo class implements the Foo widget for FLTK.

                                  This description text appears in the documentation for
                                  the class and may include HTML tags as desired.
                                */

                                class FL_EXPORT Foo : public Widget {
                                  int private_data_;

                                public:
                                  /**
                                    Creates a Foo widget with the given position and label.

                                    This description text appears in the documentation for the
                                    method's implementation.

                                    References to parameters \p X, \p Y, \p W, \p H are
                                    mentioned this way.

                                    \param[in] X,Y,W,H Position and size of widget
                                    \param[in] L Optional label (default is 0 for no label)
                                  */
                                  Foo(int X, int Y, int W, int H, const char *L = 0) {
                                    ..implementation here..
                                  }
                                };
                                

                                
                                    Essentially, a comment starting with 
                                    /**
                                     before the
                                    class or method defines the documentation for that class or
                                    method. These comments should appear in the header file for
                                    classes and inline methods and in the code file for non-inline
                                    methods.
                                

                                In addition to Doxygen comments, block comments must be used
                                liberally in the code to describe what is being done. If what
                                you are doing is not "intuitively obvious to a casual observer",
                                add a comment! Remember, you're not the only one that has to
                                read, maintain, and debug the code.

                                
                                    Never
                                     use C++ comments in C code files or in header
                                    files that may be included from a C program. (Otherwise builds
                                    on strict platforms like SGI will fail).

                                    Normally, fltk C files have ".c" and ".h" file extensions, and C++ have
                                    ".cxx" and ".H". Currently there are a few exceptions; filename.H and
                                    Fl_Exports.H both get interpreted by C and C++, so you 
                                    must
                                     use C style
                                    comments in those.

                                ## 
                                    [General Developer Recommendations](null)
                                    [*](#DR_GENERAL_DEV_RECO)
                                
                                

                                        Most important rule: 
                                    <b>Put Doxygen comments where the code's implementation is.</b>

                                        This means don't put the docs with the prototypes in the .H file, unless that's where
                                        the code is implemented.
                                        
                                    <p>
                                        *  class, typedef, enum, and inline docs go in the headers
                                        *  Most other docs go in the source files
                                        *  For doxygen syntax in C++ files, use:
                                            
                                    <pre class='command'>
                                        /** for standard doxygen comments */
                                        ///&lt; for short single line post-declaration doxygen comments
                                        </pre>
                                    
                                    * 
                                         For doxygen syntax in C files, use:
                                            
                                        <pre class='command'>
                                            /**  for standard doxygen comments */
                                            /**&lt; for short single line post-declaration doxygen comments */
                                            </pre>
                                    
                                    * 
                                         Use 
                                        <kbd>\p</kbd>
                                         for parameters citation in the description
                                    
                                    * 
                                         Use 
                                        <kbd>\param[in] xxx</kbd>
                                         and 
                                        <kbd>\param[out] xxx</kbd>
                                         for input/output parameters.
                                    
                                    * 
                                         Don't use doxygen tags between the 
                                        <kbd>\htmlonly</kbd>
                                         and 
                                        <kbd>\endhmltonly</kbd>
                                         pair of tags.
                                    
                                    * 
                                         When commenting out code or writing non-doxygen comments, be sure not to accidentally
                                                 use doxygen comment styles, or your comments will be published..! Beware doxygen recognizes
                                                 other comment styles for itself:
                                            
                                        <pre class='command'>
                                            /*! beware */
                                            /*@ beware */
                                            //! beware
                                            //@ beware</pre>

                                            There may be others. For this reason, 
                                        _follow all non-doxygen comment leaders with a space_

                                            to avoid accidental doxygen parsing:
                                            
                                        <p>
                                        <table>
                                            <tr>
                                                <td>
                                                    <pre class='command'>

                                                            /* safe from doxygen */
                                                            // safe from doxygen
                                                              ^
                                                             /|\
                                                              |
                                                              
                                                        _Space immediately after comment characters_
                                                    </pre>
                                                </td>
                                            </tr>
                                        </table>
                                        <p>
                                    
                                    * 
                                         Note: Several characters are 'special' within doxygen commments, and must be escaped
                                            with a backslash to appear in the docs correctly. Some of these are:
                                            
                                        <pre class='command'>
                                            \&lt;    -- disambiguates html tags
                                            \&gt;    -- ""
                                            \&amp;    -- ""
                                            \@    -- disambiguates JavaDoc doxygen comments
                                            \$    -- disambiguates environment variable expansions
                                            \#    -- disambiguates references to documented entities
                                            \%    -- prevents auto-linking
                                            \\    -- escapes the escape character</pre>
                                    
                                    *  Itemized lists can be specified two ways; both work, left is preferred:
                                    <p>
                                    <table border="1">
                                        <tr>
                                            <th align=center>- Preferred -</th>
                                            <th align=center>- Old -</th>
                                        </tr>
                                        <tr>
                                            <td valign=top>
                                                <pre class='command'>

                                                    /** Here's a bullet list:      <!-- space -->


                                                          - Apples
                                                          - Oranges

                                                       Here's a numbered list:

                                                          -# First thing
                                                          -# Second thing

                                                     */
                                                </pre>
                                            </td>
                                            <td valign=top>
                                                <pre class='command'>

                                                    /** Here's a bullet list:      <!-- space -->

                                                        &lt;ul&gt;
                                                            &lt;li&gt; Apples&lt;/li&gt;
                                                            &lt;li&gt; Oranges&lt;/li&gt;
                                                        &lt;/ul&gt;
                                                        Here's a numbered list:
                                                        &lt;ol&gt;
                                                            &lt;li&gt; First thing&lt;/li&gt;
                                                            &lt;li&gt; Second thing&lt;/li&gt;
                                                        &lt;ol&gt;
                                                     */
                                                </pre>
                                            </td>
                                        </tr>
                                    </table>
                                

                                ### Documenting Temporary Code or Issues

                                Temporary code and code that has a known issue MUST be
                                documented in-line with the following (Doxygen) comment style:

                                
                                    /** \todo this code is temporary */
                                

                                
                                    \todo
                                     items are listed by Doxygen making it easy to
                                    locate any code that has an outstanding issue or code that
                                    should be removed or commented out prior to a release.
                                

                                ### Documenting Classes and Structures

                                Classes and structures start with a comment block that looks
                                like the following:

                                
                                     /**
                                       A brief description of the class/structure.

                                       A complete description of the class/structure.
                                     */
                                     class MyClass {
                                     };
                                

                                ### Documenting Enumerations

                                Enumerations start with a comment block that looks like the
                                following:

                                
                                     /**
                                       A brief description of the enumeration.

                                       A complete description of the enumeration.
                                     */
                                     enum MyEnum {
                                       ...
                                     };
                                

                                Each enumeration value must be documented in-line next to the
                                corresponding definition as follows:
                                /* C++ STYLE */
                                enum MyEnum {
                                  BLACK,    ///&lt; The color black.
                                  RED,      ///&lt; The color red.
                                  GREEN,    ///&lt; The color green.
                                  YELLOW,   ///&lt; The color yellow.
                                  BLUE,     ///&lt; The color blue.
                                };
                                
                                If the enum is included in a C file, be sure to use C style commenting:
                                /* C STYLE */
                                enum MyEnum {
                                  BLACK,    /**&lt; The color black. */
                                  RED,      /**&lt; The color red. */
                                  GREEN,    /**&lt; The color green. */
                                  YELLOW,   /**&lt; The color yellow. */
                                  BLUE,     /**&lt; The color blue. */
                                };

                                ### Documenting Functions and Methods

                                Functions and methods start with a comment block that looks
                                like the following:
                                
                                

                                    /**
                                      A brief description of the function/method.

                                      A complete description of the function/method.
                                      Optional passing mention of parameter 
                                    \p a
                                     and 
                                    \p out1
                                    .

                                      Optional code example goes here if needed:
                                      
                                    \code

                                      ..code showing how to use it..
                                      
                                    \endcode

                                    \param[in] a
                                     Description of input variable a
                                      
                                    \param[in] x,y
                                     Description of input variables x and y in one comment
                                      
                                    \param[out] out1
                                     Description of output variable out1
                                      
                                    \param[out] out2
                                     Description of output variable out2
                                      
                                    \return
                                     0 on success, -1 on error
                                      
                                    \see
                                     other_func1(), other_func2()
                                    */
                                    int my_function(int a, int x, int y, float &amp;out1, float &amp;out2) {
                                      ...implementation...
                                    }
                                

                                Some details on the above:
                                
                                    Parameters
                                

                                           Use 
                                    <b>\param</b>
                                     to document function/method parameters
                                           using either of the following formats, the latter being preferred:
                                    <p>
                                    <pre class='command'>
                                        <b>\param var</b>
                                         Some description
                                           
                                        <b>\param[in|out] var</b>
                                         Some description.
                                               
                                    </pre>

                                           Mention of parameters in docs should use "\p varname". (Use \p instead of \a)
                                           
                                    <p>

                                               Note: Doxygen checks your \param variable names against the actual function signatures
                                               in your code. It does 
                                        <u>NOT</u>
                                         check \p names for consistency.
                                               
                                    <p>
                                

                                Return Values
                                

                                          Use 
                                    <b>\return</b>
                                     to document return values. Omit this if there is no return value.
                                

                                Reference related methods
                                
                                    <p>

                                              Use 
                                        <b>\see</b>
                                         to help the reader find related methods.
                                              (Methods are sorted alphabetically by doxygen, so 'related' methods might not
                                              appear together)
                                              
                                    <p>

                                              Locate 
                                        <b>\see</b>
                                         references below \param[] and \return as shown
                                              in the above example.
                                

                                Code examples
                                

                                          Use 
                                    <b>\code</b>
                                     and 
                                    <b>\endcode</b>
                                     when code examples are needed.
                                          Text within will be exempt from html and doxygen escape code parsing,
                                          so you don't have to escape characters &lt;, &gt;, &amp;, etc.
                                          as you would normally.
                                          
                                    <p>

                                              Be careful not to embed C style comments within 
                                        <b>\code</b>

                                              or it will break the outer doxygen comment block. (A good reason
                                              to always test build the code base before commiting documentation-only mods)
                                

                                Where to put docs
                                
                                      Function/method documentation must be placed next to the corresponding code.
                                      (Rule: "Put the docs where the code implementation is.")
                                      Comments for in-line functions and methods are placed in the header file where they're defined.
                                

                                ### Documenting Members and Variables


                                Members and variables can be documented in one of two ways; in block comment form:

                                
                                /**
                                  A brief doxygen description of the member/variable.

                                  A complete description of the member/variable.
                                  More text goes here..
                                */
                                int my_variable_;
                                


                                or in the preferred form as in-line comments as follows:

                                
                                int my_variable1_;    ///&lt; C++ file's brief doxygen description of the member/variable
                                int my_variable2_;    /**&lt; C file's brief doxygen description of the member/variable */
                                

                                ## 
                                    [Methodology, Algorithms, Etc.](null)
                                

                                
                                    The goal of FLTK is to provide a robust GUI toolkit that is
                                    small, fast, and reliable. 
                                    All
                                     public API functions and
                                    methods must be documented with the valid values for all input
                                    parameters - 
                                    `NULL`
                                     pointers, number ranges, etc. -
                                    and no public API function may have undefined behaviors. Input
                                    validation should be performed only when the function or method
                                    is able to return an error to the caller.

                                When solving a particular problem, whether you are writing a
                                widget or adding functionality to the library, please consider
                                the following guidelines:

                                

                                    1. Choose the small, simple, easy-to-test algorithm
                                            over a more complex, larger one that is harder to debug
                                            and maintain.

                                            
                                    1. Choose the fastest algorithm that satisfies #1

                                            
                                    1. Break complex solutions into smaller, more
                                            manageable pieces.

                                            
                                    1. If functionality can be separated from the main part
                                            of the FLTK library, separate it. The idea is to keep
                                            the FLTK "core" as small as possible so that programs
                                            use memory proportionate to their complexity rather than
                                            starting big.

                                            
                                    1. 
                                        Choose a general-purpose solution over a
                                                single-purpose solution, i.e. don't limit your design to
                                                what 
                                        <I>you</I>
                                         think something will be used for.

                                                
                                    1. Don't rely on functionality available on a
                                            particular platform or compiler; this ties in with #5.

                                

                                ## 
                                    [C++ Portability](null)
                                

                                Since FLTK is targeted at platforms which often lack complete
                                ISO C++ support or have limited memory, all C++ code in FLTK
                                must use a subset of ISO C++. These restrictions shall be
                                reviewed prior to each minor or major release of FLTK.

                                ### 
                                    [FLTK 1.1.x Restrictions](null)
                                

                                The following C++ features may be not used in FLTK 1.1.x code:

                                
                                    * Exceptions
                                    * Namespaces
                                    * Standard C++ headers and library
                                    * Templates
                                    * 
                                        <kbd>static_cast</kbd>
                                        , 
                                        <kbd>dynamic_cast</kbd>
                                        , 
                                        <kbd>const_cast</kbd>
                                    
                                

                                ### 
                                    [FLTK 1.3.x Restrictions](null)
                                

                                The following C++ features may be not used in FLTK 1.3.x code:

                                
                                    * Exceptions
                                    * Namespaces
                                    * Standard C++ headers and library
                                    * Templates
                                    * 
                                        <kbd>dynamic_cast</kbd>
                                    
                                

                                ### 
                                    [FLTK 1.4.x Restrictions](null)
                                

                                The following C++ features may be not used in FLTK 1.4.x code:

                                
                                    * Exceptions
                                    * Namespaces
                                    * Standard C++ headers and library
                                    * Templates
                                    * 
                                        <kbd>dynamic_cast</kbd>
                                    
                                


                                    The 
                                reinterpret_cast
                                 keyword may be used but is
                                    not mandatory.
                                    
                                

                                    The 
                                static_cast
                                 and 
                                const_cast
                                 keywords
                                    should be used when casting pointers of different types.
                                    
                                

                                    The 
                                dynamic_cast
                                 keyword must not be used since
                                    run-time typing features may not be available at all times.

                                ## 
                                    [Source File Naming](null)
                                

                                The current practice is to use an extension of ".c" for C
                                source files, ".h" for C header files, ".cxx" for C++ source
                                files, and ".H" for C++ header files in the "FL" directory (".h"
                                otherwise.)

                                ## 
                                    [Function/Method/Variable Naming](null)
                                

                                All public (exported) functions and variables must be placed
                                in the "fltk" namespace. Except for constructor and destructor
                                methods, the names consist of lowercase words separated by the
                                underscore ("_"), e.g. "fltk::some_variable" and "text_color()".
                                Private member variables of classes end with an extra
                                underscore, e.g. "text_size_".

                                ## 
                                    [Structure/Class Naming](null)
                                

                                All public (exported) structures and classes must be placed
                                in the "fltk" namespace and consist of capitalized words without
                                the underscore, e.g. "fltk::SuperWidget".

                                Private members of classes must end with a trailing
                                underscore ("_") and have corresponding public access methods
                                without the underscore as applicable, e.g. "text_size_" and
                                "text_size()".

                                ## 
                                    [Constant/Enumeration Naming](null)
                                

                                
                                    Public enumerations and constant variables must be placed
                                    inside the "fltk" namespace and consist of UPPERCASE WORDS
                                    separated by the underscore ("_"), e.g. "ALIGN_LEFT",
                                    "COLOR_RED", etc. Enumeration type names consist of capitalized
                                    words without underscores, e.g. "MyEnum". 
                                    #define

                                    constants are prohibited aside from the include guard
                                    definitions.


                                ## 
                                    [Preprocessor Variables](null)
                                


                                File config.h and the C++ compilers define a few preprocessor variables
                                that help organizing platform-specific code
                                and control access to a few internal classes. Only code internal
                                to the FLTK library can include the config.h header file. Thus,
                                FLTK header files that are part of the public API
                                must not, directly or indirectly, include config.h.
                                
                                <ul type=disc>
                                    * 
                                        <a name="_WIN32"></a>
                                        _WIN32 identifies the MS-Windows platform (both
                                            for the 32- and 64-bit versions). Note: FLTK 1.3.x used 
                                        <kbd>WIN32</kbd>

                                            which had to be defined by the FLTK build system whereas FLTK 1.4 uses
                                            
                                        <kbd>_WIN32</kbd>
                                         (with leading underscore) which should be defined by
                                            the build tools (preprocessor, compiler) on the Windows platform.

                                            
                                    * 
                                        <a name="__CYGWIN__"></a>
                                        __CYGWIN__ is defined when FLTK runs on Windows
                                            but uses Cygwin's POSIX emulation features (cygwin1.dll).

                                            
                                    * 
                                        <a name="__APPLE__"></a>
                                        __APPLE__ identifies the macOS platform.

                                            
                                    * 
                                        <a name="__APPLE_QUARTZ__"></a>
                                        __APPLE_QUARTZ__ is defined by config.h for
                                            the macOS platform. At present, use of __APPLE_QUARTZ__ is equivalent to
                                            using __APPLE__. This may change in the future if other graphics
                                            systems than Quartz are supported on the macOS platform.

                                            
                                    * 
                                        <a name="USE_X11"></a>
                                        USE_X11 is defined by config.h when Xlib is the
                                            graphics system used. Thus, USE_X11 is defined on all Unix and Linux
                                            platforms, and on Windows, if configure used 
                                        <kbd>--enable-cygwin</kbd>
                                        <b>and</b>
                                        <kbd>--enable-x11</kbd>
                                        . Xlib-specific code is also often
                                            delimited without reference to the USE_X11 variable (thus without the
                                            requirement to include config.h) as follows:
                                        <pre>#if defined(WIN32)
                                        #elif defined(__APPLE__)
                                        #else
                                        .. Xlib specific code ...
                                        #endif
                                        </pre>

                                    * 
                                        <a name="USE_XFT"></a>
                                        USE_XFT is defined by config.h when USE_X11 is defined.
                                            It is set to 1 when the Xft library of scalable, anti-aliased
                                            fonts is used, and to 0 otherwise.

                                            
                                    * 
                                        <a name="FL_LIBRARY"></a>
                                        FL_LIBRARY is defined by all FLTK library build
                                            systems when the FLTK library itself is compiled.
                                            Application program developers should not define it when compiling
                                            their programs.

                                            
                                    * 
                                        <a name="FL_DLL"></a>
                                        FL_DLL is defined by the FLTK build system
                                            when building shared libraries (DLL's) with Visual Studio. Application
                                            program developers using Visual Studio 
                                        <b>and</b>
                                         linking against the
                                            shared FLTK libraries (DLL's) built with Visual Studio must define this
                                            macro when compiling their source files. Note that this macro must be
                                            defined by the build system (VS project setup/properties) or on the
                                            compiler command line so it is "seen" by all included FLTK header files.

                                            
                                    * 
                                        <a name="FL_INTERNALS"></a>
                                        FL_INTERNALS. Application program developers
                                            can define this variable to get access to some internal classes (e.g., the
                                            Fl_X class) if they need it. APIs to these internal classes are
                                            highly subject to changes, though.

                                            
                                    * 
                                        <a name="FL_DOXYGEN"></a>
                                        FL_DOXYGEN is defined when the Doxygen program
                                            that builds the FLTK documentation processes the source code. This variable
                                            has two major uses.
                                            
                                        <ol>
                                            * 
                                                <a name="XXX"></a>
                                                <kbd>#ifndef FL_DOXYGEN / #endif</kbd>
                                                 allows to hide code from Doxygen.
                                                      
                                            * 
                                                The Doxygen program does not define the platform-specific
                                                        variables 
                                                <kbd>__APPLE__</kbd>
                                                 or 
                                                <kbd>_WIN32</kbd>
                                                 (even if it's run on macOS or Windows).
                                                        Thus, platform-specific (say, macOS-specific) code must be bracketed as
                                                        follows to be seen by Doxygen:
                                                <pre>#if defined(__APPLE__) || defined(FL_DOXYGEN)
                                                ... Doxygen-visible, macOS-specific code ...
                                                #endif
                                                </pre>
                                        </ol>
                                    
                                    * 
                                        <a name="FL_ABI_VERSION">FL_ABI_VERSION</a>
                                         is used to allow developers
                                            to implement ABI breaking code in 
                                        <a href="#SR_PATCH_RELEASES">Patch Releases</a>
                                        .
                                            Normally set to the default ABI version for each minor version (for instance 10400 for
                                            all 1.4.x releases), can be changed by users or devs with configure or CMake to
                                            enable ABI breaking features for testing or use by end users in static builds of FLTK.
                                            
                                        <p>
                                            Note: This preprocessor variable was named FLTK_ABI_VERSION in FLTK 1.3.x
                                            and was renamed to FL_ABI_VERSION since FLTK 1.4.0.
                                            
                                        <p>
                                            When set, the variable's value is expected to be the integer representation
                                            of the FLTK version number, where the Minor and Patch numbers are padded to
                                            two digits to allow for numbers 1 thru 99, e.g.
                                            
                                        <pre class='command'>
                                            #define FL_ABI_VERSION 10401    // FLTK ABI version 1.4.1
                                                ..'1' is the major version (no padding; avoids octal issues)
                                                ..'04' is the minor version (2 digit padding)
                                                ..'01' is the patch version (2 digit padding)
                                            </pre>

                                            ABI breaking features are by default '#ifdef'ed out with this variable
                                            during patch releases, and are merged in by developers during the next
                                            
                                        <a href="#SR_MINOR_RELEASES">Minor Release</a>
                                        .
                                            
                                        <p>
                                            Example: If the current patch release is 1.4.0, and the developer adds an
                                            ABI-breaking fix to what will be the next 1.4.1 release, then the new
                                            code would be implemented as:
                                            
                                        <p>
                                        
                                            <table>
                                                <tr>
                                                    <td>
                                                        <pre class='command'>

                                                                #if FL_ABI_VERSION &gt;= 10401  // FLTK 1.4.1, the 
                                                            <u>next</u>
                                                             patch release #
                                                                    ... new ABI breaking code ...
                                                                #else
                                                                    ... old non-ABI breaking (default builds) ...
                                                                #endif
                                                        </pre>
                                                    </td>
                                                </tr>
                                            </table>
                                        


                                            This variable solves several issues:
                                            
                                        
                                        <ul type=disc>
                                            *  Allows ABI breaking code to be implemented at any time by developers.
                                            *  Gets fixes into Git sooner, so users can see, test and access it.
                                            *  Testing ABI features during Patch Releases increases the stability of Minor Releases.
                                            *  Prevents devs having to defer ABI breaking code to the small window of time preceding Minor Releases.
                                        
                                    
                                

                                ## 
                                    [Miscellaneous](null)
                                
                                ## 
                                    [Using switch() /* FALLTHROUGH */](null)
                                
                                

                                        When using 
                                    <kbd>switch</kbd>
                                     - 
                                    <kbd>case</kbd>
                                     statements, and your
                                        
                                    <kbd>case</kbd>
                                     statement does not end in 
                                    <kbd>break</kbd>
                                     in order to
                                        fall through to the next 
                                    <kbd>case</kbd>
                                     statement, the comment
                                        
                                    <kbd>/* FALLTHROUGH */</kbd>
                                     should be added where the 
                                    <kbd>break</kbd>

                                        statement would be.
                                

                                ## 
                                    [Useful Visual Studio C++ Macros (Windows)](null)
                                
                                

                                        Here's a list of Visual Studio compiler macros that can be used
                                        to conditionalize code based on the Visual Studio version:
                                    <pre>
                                        VERSION     MACRO            PRODUCT NAME
                                        -------     ---------------- --------------------
                                        MSVC++ 16.0 _MSC_VER == 1920 Visual Studio 2019
                                        MSVC++ 15.0 _MSC_VER == 1910 Visual Studio 2017
                                        MSVC++ 14.0 _MSC_VER == 1900 Visual Studio 2015
                                        MSVC++ 12.0 _MSC_VER == 1800 Visual Studio 2013
                                        MSVC++ 11.0 _MSC_VER == 1700 Visual Studio 2012
                                        MSVC++ 10.0 _MSC_VER == 1600 Visual Studio 2010
                                        MSVC++ 9.0  _MSC_VER == 1500 Visual Studio 2008
                                        MSVC++ 8.0  _MSC_VER == 1400 Visual Studio 2005
                                        MSVC++ 7.1  _MSC_VER == 1310 Visual Studio 2003
                                        MSVC++ 7.0  _MSC_VER == 1300 Visual Studio 7
                                        MSVC++ 6.0  _MSC_VER == 1200 Visual Studio 6
                                        MSVC++ 5.0  _MSC_VER == 1100 Visual Studio 5
                                    </pre>

                                        Recommended usage:
                                    <pre>
                                        #if defined(_MSC_VER) &amp;&amp; (_MSC_VER &lt;= 1300)             /* VS7 and older */
                                          ..
                                        #else  /* _MSC_VER */
                                          ..
                                        #endif /* _MSC_VER */
                                    </pre>
                                
                                ## 
                                    [Useful Xcode C++ Macros (macOS)](null)
                                
                                

                                        Here's a list of operating system version compiler macros
                                        that can be used to conditionalize code based on the compile time
                                        version of the macOS operating system.
                                        
                                    <p>

                                            These are made available from Apple's 
                                        <kbd>AvailabilityMacros.h</kbd>
                                        .
                                            For more info on these and other macros, see Apple's "TechNote 2064".
                                    <pre>
                                        VERSION     MACRO                   VALUE    PRODUCT NAME
                                        -------     ---------------------   -----    --------------------
                                        10.0        MAC_OS_X_VERSION_10_0    1000    Cheetah
                                        10.1        MAC_OS_X_VERSION_10_1    1010    Puma
                                        10.2        MAC_OS_X_VERSION_10_2    1020    Jaguar
                                        10.3        MAC_OS_X_VERSION_10_3    1030    Panther
                                        10.4        MAC_OS_X_VERSION_10_4    1040    Tiger
                                        10.5        MAC_OS_X_VERSION_10_5    1050    Leopard
                                        10.6        MAC_OS_X_VERSION_10_6    1060    Snow Leopard
                                        10.7        MAC_OS_X_VERSION_10_7    1070    Lion
                                        10.8        MAC_OS_X_VERSION_10_8    1080    Mountain Lion
                                        10.9        MAC_OS_X_VERSION_10_9    1090    Mavericks
                                        10.10       MAC_OS_X_VERSION_10_10 101000    Yosemite
                                        10.11       MAC_OS_X_VERSION_10_11 101100    El Capitan
                                        10.12       MAC_OS_X_VERSION_10_12 101200    Sierra
                                        etc..
                                    </pre>

                                        Recommended usage:
                                    <pre>
                                        #include &lt;FL/platform.H&gt; // defines the MAC_OS_X_VERSION_10_xx macros
                                        #if (MAC_OS_X_VERSION_MAX_ALLOWED &gt;= MAC_OS_X_VERSION_10_5)
                                        if (fl_mac_os_version &gt;= 100500) {
                                          ..10.5 and newer..
                                        } else
                                        #else
                                        {
                                          ..10.4 and older..
                                        }
                                        #endif
                                    </pre>

                                    This way, code compiled before and after macOS 10.5 will be able to run on
                                    computers running macOS versions before and after 10.5.
                                
                                ## 
                                    [Useful GNU C++ Macros](null)
                                
                                
                                   TBD.
                                

                                # 
                                    [Makefile Standards](null)
                                

                                The following is a guide to the makefile-based build system
                                used by FLTK. These standards have been developed over the years
                                to allow FLTK to be built on as many systems and environments as
                                possible.

                                ## 
                                    [General Organization](null)
                                

                                The FLTK source code is organized functionally into a
                                top-level makefile, include file, and subdirectories each with
                                their own makefile and depedencies files:

                                
                                    Makefile.in
                                    configh.in
                                    configure.in
                                    makeinclude.in

                                    FL
                                        Makefile.in
                                        *.H

                                    fltk
                                        *.h

                                    fluid
                                        Makefile
                                        makedepend
                                        *.h
                                        *.c
                                        *.cxx

                                    src
                                        Makefile
                                        makedepend
                                        *.h
                                        *.c
                                        *.cxx

                                    test
                                        Makefile
                                        makedepend
                                        *.h
                                        *.c
                                        *.cxx
                                

                                
                                    The ".in" files are template files for the 
                                    autoconf

                                    and 
                                    CMake
                                     software and are used to generate a static version
                                     of the corresponding file.
                                

                                ## 
                                    [Makefile Documentation](null)
                                

                                
                                    Each Makefile must start with the standard FLTK header
                                    containing a description of the file, and the FLTK copyright/license notice.
                                    
                                    Note: As of July 2020, we no longer use Subversion "&#36;Id$" keywords or 'End of' trailers.
                                    

                                    Example:

                                
                                    #
                                    # Some descriptive text for the Fast Light Tool Kit (FLTK).
                                    #
                                    # Copyright 1998-2020 by Bill Spitzak and others.
                                    #
                                    # This library is free software; you can redistribute it and/or
                                    # modify it under the terms of the GNU Library General Public
                                    # License as published by the Free Software Foundation; either
                                    # version 2 of the License, or (at your option) any later version.
                                    #
                                    # This library is distributed in the hope that it will be useful,
                                    # but WITHOUT ANY WARRANTY; without even the implied warranty of
                                    # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
                                    # Library General Public License for more details.
                                    #
                                    # You should have received a copy of the GNU Library General Public
                                    # License along with this library; if not, write to the Free Software
                                    # Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
                                    # USA.
                                    #
                                    # Please see the following page on how to report bugs and issues:
                                    #
                                    #     https://www.fltk.org/bugs.php
                                    #
                                

                                ## 
                                    [Portable Makefile Construction](null)
                                

                                FLTK uses a common subset of make program syntax to ensure
                                that the software can be compiled "out of the box" on as many
                                systems as possible. The following is a list of assumptions we
                                follow when constructing makefiles:

                                

                                    * 
                                        <b>Targets</b>
                                        ; we assume that the make program
                                                supports the notion of simple targets of the form
                                                "name:" that perform tab-indented commands that follow
                                                the target, e.g.:
                                                
                                        <pre>

                                                target:
                                                
                                            |IMG|
                                             target commands
                                        </pre>
                                    

                                    * 
                                        <b>Dependencies</b>
                                        ; we assume that the make program
                                                supports recursive dependencies on targets, e.g.:
                                                
                                        <pre>

                                                target: foo bar
                                                
                                            |IMG|
                                             target commands

                                                foo: bla
                                                
                                            |IMG|
                                             foo commands

                                                bar:
                                                
                                            |IMG|
                                             bar commands

                                                bla:
                                                
                                            |IMG|
                                             bla commands
                                        </pre>
                                    

                                    * 
                                        <b>Variable Definition</b>
                                        ; we assume that the make program
                                                supports variable definition on the command-line or in the makefile
                                                using the following form:
                                                
                                        <pre>
                                            name=value</pre>

                                    * 
                                        <b>Variable Substitution</b>
                                        ; we assume that the make program
                                                supports variable substitution using the following forms:
                                                
                                        <ul>
                                            * 
                                                <kbd>$(name)</kbd>
                                                ; substitutes the value of "name",
                                            
                                            * 
                                                <kbd>($name:.old=.new)</kbd>
                                                ; substitutes the value of "name"
                                                                with the filename extensions ".old" changed to ".new",
                                            
                                            * 
                                                <kbd>$(MAKEFLAGS)</kbd>
                                                ; substitutes the
                                                                command-line options passed to the program
                                                                without the leading hyphen (-),
                                            
                                            * 
                                                <kbd>$</kbd>
                                                ; substitutes a single 
                                                <kbd>$</kbd>
                                                 character,
                                            
                                            * 
                                                <kbd>

                                    * 
                                        <kbd>AR</kbd>
                                        ; the library archiver command,
                                    

                                    * 
                                        <kbd>ARFLAGS</kbd>
                                        ; options for the library archiver command,
                                    

                                    * 
                                        <kbd>BUILDROOT</kbd>
                                        ; optional installation prefix,
                                    

                                    * 
                                        <kbd>CAT1EXT</kbd>
                                        ; extension for formatted man pages in section 1,
                                    

                                    * 
                                        <kbd>CAT3EXT</kbd>
                                        ; extension for formatted man pages in section 3,
                                    

                                    * 
                                        <kbd>CC</kbd>
                                        ; the C compiler command,
                                    

                                    * 
                                        <kbd>CFLAGS</kbd>
                                        ; options for the C compiler command,
                                    

                                    * 
                                        <kbd>CXX</kbd>
                                        ; the C++ compiler command,
                                    

                                    * 
                                        <kbd>CXXFLAGS</kbd>
                                        ; options for the C++ compiler command,
                                    

                                    * 
                                        <kbd>DOXYGEN</kbd>
                                        ; the 
                                        <kbd>doxygen</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>DSOCOMMAND</kbd>
                                        ; the shared library building command,
                                    

                                    * 
                                        <kbd>EXEEXT</kbd>
                                        ; the extension for executable programs,
                                    

                                    * 
                                        <kbd>FLUID</kbd>
                                        ; the FLUID executable to install,
                                    

                                    * 
                                        <kbd>GLDEMOS</kbd>
                                        ; the OpenGL demos to build,
                                    

                                    * 
                                        <kbd>GLLIBS</kbd>
                                        ; libraries for OpenGL programs,
                                    

                                    * 
                                        <kbd>HTMLDOC</kbd>
                                        ; the 
                                        <kbd>htmldoc</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>IMAGEDIRS</kbd>
                                        ; image library directories to build,
                                    

                                    * 
                                        <kbd>IMGLIBS</kbd>
                                        ; libraries for image programs,
                                    

                                    * 
                                        <kbd>INSTALL</kbd>
                                        ; the 
                                        <kbd>install</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>INSTALL_BIN</kbd>
                                        ; the program installation command,
                                    

                                    * 
                                        <kbd>INSTALL_DATA</kbd>
                                        ; the data file installation command,
                                    

                                    * 
                                        <kbd>INSTALL_DIR</kbd>
                                        ; the directory installation command,
                                    

                                    * 
                                        <kbd>INSTALL_LIB</kbd>
                                        ; the library installation command,
                                    

                                    * 
                                        <kbd>INSTALL_MAN</kbd>
                                        ; the documentation installation command,
                                    

                                    * 
                                        <kbd>INSTALL_SCRIPT</kbd>
                                        ; the shell script installation command,
                                    

                                    * 
                                        <kbd>LDFLAGS</kbd>
                                        ; options for the linker,
                                    

                                    * 
                                        <kbd>LIBNAME</kbd>
                                        ; the name of the FLTK library to install,
                                    

                                    * 
                                        <kbd>LIBS</kbd>
                                        ; libraries for all programs,
                                    

                                    * 
                                        <kbd>LN</kbd>
                                        ; the 
                                        <kbd>ln</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>MAKEDEPEND</kbd>
                                        ; the 
                                        <kbd>makedepend</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>MKDIR</kbd>
                                        ; the 
                                        <kbd>mkdir</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>NROFF</kbd>
                                        ; the 
                                        <kbd>nroff</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>OPTIM</kbd>
                                        ; common compiler optimization options,
                                    

                                    * 
                                        <kbd>POSTBUILD</kbd>
                                        ; the post build command to run (macOS),
                                    

                                    * 
                                        <kbd>RM</kbd>
                                        ; the 
                                        <kbd>rm</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>RMDIR</kbd>
                                        ; the 
                                        <kbd>rmdir</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>SHAREDLIBS</kbd>
                                        ; shared libraries for installed programs,
                                    

                                    * 
                                        <kbd>SHELL</kbd>
                                        ; the 
                                        <kbd>sh</kbd>
                                         (POSIX shell) command,
                                    

                                    * 
                                        <kbd>STRIP</kbd>
                                        ; the 
                                        <kbd>strip</kbd>
                                         command,
                                    

                                    * 
                                        <kbd>THREADS</kbd>
                                        ; the threading demos to build,
                                    

                                    * 
                                        <kbd>bindir</kbd>
                                        ; the binary installation directory,
                                    

                                    * 
                                        <kbd>datadir</kbd>
                                        ; the data file installation directory,
                                    

                                    * 
                                        <kbd>exec_prefix</kbd>
                                        ; the installation prefix for executable files,
                                    

                                    * 
                                        <kbd>libdir</kbd>
                                        ; the library installation directory,
                                    

                                    * 
                                        <kbd>mandir</kbd>
                                        ; the man page installation directory,
                                    

                                    * 
                                        <kbd>prefix</kbd>
                                        ; the installation prefix for non-executable files, and
                                    

                                    * 
                                        <kbd>srcdir</kbd>
                                        ; the source directory.
                                    

                                lt;</kbd>
                                                ; substitutes the current source file or dependency, and
                                            
                                            * 
                                                <kbd>$@</kbd>
                                                ; substitutes the current target name.
                                            
                                        
                                    

                                    
                                        Suffixes
                                        ; we assume that the make program
                                                supports filename suffixes with assumed dependencies, e.g.:
                                                
                                        

                                                .SUFFIXES: .c .o
                                                .c.o:
                                                
                                            ![Tab](images/tab.png)
                                             $(CC) $(CFLAGS) -o $@ -c $&lt;
                                        
                                    

                                    
                                        Include Files
                                        ; we assume that the make program
                                                supports the 
                                        include
                                         directive, e.g.:
                                                
                                        
                                            include ../makeinclude
                                            include makedepend
                                    

                                    
                                        Comments
                                        ; we assume that comments begin with
                                                a 
                                        #
                                         character and proceed to the end of the
                                                current line.
                                    

                                    
                                        Line Length
                                        ; we assume that there is no
                                                practical limit to the length of lines.
                                    

                                    
                                        Continuation of long lines
                                        ; we assume that
                                                the 
                                        \
                                         character may be placed at the end of a
                                                line to concatenate two or more lines in a
                                                makefile to form a single long line.
                                    

                                    
                                        Shell
                                        ; we assume a POSIX-compatible shell is
                                                present on the build system.
                                    

                                

                                ## 
                                    [Standard Variables](null)
                                

                                
                                    The following variables are defined in the "makeinclude" file
                                    generated by the 
                                    autoconf
                                     software:

                                

                                    * 
                                        <kbd>all</kbd>
                                        ; creates all target programs,
                                                libraries, and documentation files,
                                    

                                    * 
                                        <kbd>clean</kbd>
                                        ; removes all target programs,
                                                libraries, documentation files, and object files,
                                    

                                    * 
                                        <kbd>depend</kbd>
                                        ; generates automatic dependencies
                                                for any C or C++ source files (also see 
                                        <a href='#DEPEND_TARGET'>"Dependencies"</a>
                                        ),
                                    

                                    * 
                                        <kbd>distclean</kbd>
                                        ; removes autoconf-generated files
                                                in addition to those removed by the "clean" target,
                                    

                                    * 
                                        <kbd>install</kbd>
                                        ; installs all distribution files in
                                                their corresponding locations (also see 
                                        <a href='#INSTALL_TARGET'>"Install/Uninstall Support"</a>
                                        ), 
                                    

                                    * 
                                        <kbd>uninstall</kbd>
                                        ; removes all distribution files from
                                                their corresponding locations (also see 
                                        <a href='#INSTALL_TARGET'>"Install/Uninstall Support"</a>
                                        ), and
                                    

                                    * 
                                        <kbd>unittest</kbd>
                                        ; runs any unit tests that have been
                                                created for the corresponding code and programs.
                                    

                                

                                ## 
                                    [Standard Targets](null)
                                

                                The following standard targets must be defined in each makefile:

                                <ul class='new'>

                                    ## 
                                        <a name='DR_COMMENT_STYLE'>Comment Style</a>
                                    

                                      Developers for 1.3 settled on the /** .. */ format for these reasons:
                                      
                                    <pre>
                                      erco 3/13/09:
                                          I do like when (*)s run down left margin of all comments; easier
                                          to differentiate comments from code in large doc blocks.
                                      matt 3/14/09:
                                          Yes, same here. I usually align them in the second column.
                                      erco 3/15/09:
                                          Duncan doesn't like (*)s down the left because it complicates
                                          paragraph reformatting.. passing that on.
                                      erco 3/15/09:
                                          Albrecht says this was already discussed and decision was *no stars*
                                          down the left, so I modified the examples here to follow this rule.
                                      erco 3/15/09:
                                          Note: fltk2 uses QT /*! style comments, whereas fltk1 uses /**
                                          as described above.  Should standard reflect this?
                                      erco 07/18/10:
                                          We seem to be going with /** style comments, no (*)s running down
                                          left margin (as per Duncan's sugg).
                                      </pre>

                                    ## 
                                        <a name='DR_GENERAL_DEV_RECO'>General Developer Recommendations</a>
                                    

                                      Many of the notes in this section are from Fabien's TODO.doc, and
                                      seem consistent with the actual fltk docs during the 1.1.x-to-1.3.x
                                      doxygenification transition.

                                

                                ## 
                                    [Object Files](null)
                                

                                Object files (the result of compiling a C or C++ source file)
                                have the extension ".o".

                                ## 
                                    [Programs](null)
                                

                                
                                    Program files (the result of linking object files and
                                    libraries together to form an executable file) have the
                                    extension specified by the 
                                    $(EXEEXT)
                                     variable. A
                                    typical program target looks like:

                                

                                        program$(EXEEXT): $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     echo Linking $@...
                                        
                                    ![Tab](images/tab.png)
                                     $(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)
                                

                                ## 
                                    [Static Libraries](null)
                                

                                Static libraries have a prefix of "lib" and the extension
                                ".a". A typical static library target looks like:

                                

                                        libname.a: $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     echo Creating $@...
                                        
                                    ![Tab](images/tab.png)
                                     $(RM) $@
                                        
                                    ![Tab](images/tab.png)
                                     $(AR) $(ARFLAGS) $@ $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     $(RANLIB) $@
                                

                                ## 
                                    [Shared Libraries](null)
                                

                                Shared libraries have a prefix of "lib" and the extension
                                ".dylib", ".sl", ".so", or "_s.a" depending on the operating
                                system. A typical shared library is composed of several targets
                                that look like:

                                

                                        libname.so: $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     echo $(DSOCOMMAND) libname.so.$(DSOVERSION) ...
                                        
                                    ![Tab](images/tab.png)
                                     $(DSOCOMMAND) libname.so.$(DSOVERSION) $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     $(RM) libname.so libname.so.$(DSOMAJOR)
                                        
                                    ![Tab](images/tab.png)
                                     $(LN) libname.so.$(DSOVERSION) libname.so.$(DSOMAJOR)
                                        
                                    ![Tab](images/tab.png)
                                     $(LN) libname.so.$(DSOVERSION) libname.so

                                        libname.sl: $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     echo $(DSOCOMMAND) libname.sl.$(DSOVERSION) ...
                                        
                                    ![Tab](images/tab.png)
                                     $(DSOCOMMAND) libname.sl.$(DSOVERSION) $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     $(RM) libname.sl libname.sl.$(DSOMAJOR)
                                        
                                    ![Tab](images/tab.png)
                                     $(LN) libname.sl.$(DSOVERSION) libname.sl.$(DSOMAJOR)
                                        
                                    ![Tab](images/tab.png)
                                     $(LN) libname.sl.$(DSOVERSION) libname.sl

                                        libname.dylib: $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     echo $(DSOCOMMAND) libname.$(DSOVERSION).dylib ...
                                        
                                    ![Tab](images/tab.png)
                                     $(DSOCOMMAND) libname.$(DSOVERSION).dylib \
                                        
                                    ![Tab](images/tab.png)
                                    ![Tab](images/tab.png)
                                     -install_name $(libdir)/libname.$(DSOMAJOR).dylib \
                                        
                                    ![Tab](images/tab.png)
                                    ![Tab](images/tab.png)
                                     -current_version libname.$(DSOVERSION).dylib \
                                        
                                    ![Tab](images/tab.png)
                                    ![Tab](images/tab.png)
                                     -compatibility_version $(DSOMAJOR).0 \
                                        
                                    ![Tab](images/tab.png)
                                    ![Tab](images/tab.png)
                                     $(OBJECTS) $(LIBS)
                                        
                                    ![Tab](images/tab.png)
                                     $(RM) libname.dylib
                                        
                                    ![Tab](images/tab.png)
                                     $(RM) libname.$(DSOMAJOR).dylib
                                        
                                    ![Tab](images/tab.png)
                                     $(LN) libname.$(DSOVERSION).dylib libname.$(DSOMAJOR).dylib
                                        
                                    ![Tab](images/tab.png)
                                     $(LN) libname.$(DSOVERSION).dylib libname.dylib

                                        libname_s.a: $(OBJECTS)
                                        
                                    ![Tab](images/tab.png)
                                     echo $(DSOCOMMAND) libname_s.o ...
                                        
                                    ![Tab](images/tab.png)
                                     $(DSOCOMMAND) libname_s.o $(OBJECTS) $(LIBS)
                                        
                                    ![Tab](images/tab.png)
                                     echo $(LIBCOMMAND) libname_s.a libname_s.o
                                        
                                    ![Tab](images/tab.png)
                                     $(RM) $@
                                        
                                    ![Tab](images/tab.png)
                                     $(LIBCOMMAND) libname_s.a libname_s.o
                                        
                                    ![Tab](images/tab.png)
                                     $(CHMOD) +x libname_s.a
                                

                                [](null)
                                ## 
                                    [Dependencies](null)
                                

                                Static dependencies are expressed in each makefile following the
                                target, for example:

                                
                                    foo: bar
                                

                                Static dependencies shall only be used when it is not
                                possible to automatically generate them. Automatic dependencies
                                are stored in a file named "makedepend" and included at the
                                end of the makefile. The following "depend" target rule shall be
                                used to create the automatic dependencies:

                                

                                        depend: $(CPPFILES) $(CFILES)
                                        
                                    ![Tab](images/tab.png)
                                     $(MAKEDEPEND) -Y -I.. -f makedepend $(CPPFILES) $(CFILES)
                                

                                We only regenerate the automatic dependencies on a Linux system and
                                express any non-Linux dependencies statically in the Makefile.

                                To regenerate the automatic dependencies run these commands:
                                
                                    make clean
                                    ./configure
                                    make
                                    make depend
                                    cd cairo
                                    make depend
                                    cd ..
                                


                                This regenerates the main 'makedepend' files for the default configuration.
                                Commit the changes as appropriate.

                                
                                    Note 1
                                    : for different configurations 
                                    users
                                     (or developers) need to
                                    regenerate the static dependencies themselves - but take care not to commit
                                    modified 'makedepend' files!

                                
                                    Note 2
                                    : static dependencies are only used for autoconf/configure/make
                                    builds, they are not used for CMake builds which generate their own dependencies.
                                    We recommend CMake builds for FLTK development but developers need to make sure
                                    that autoconf/configure/make builds still work.

                                    [](null)
                                ## 
                                    [Install/Uninstall Support](null)
                                

                                
                                    All makefiles must contain install and uninstall rules which
                                    install or remove the corresponding software. These rules must
                                    use the 
                                    $(BUILDROOT)
                                     variable as a prefix to any
                                    installation directory so that FLTK can be installed in a
                                    temporary location for packaging by programs like
                                    rpmbuild
                                    .

                                
                                    The 
                                    $(INSTALL_BIN)
                                    , 
                                    $(INSTALL_DATA)
                                    ,
                                    $(INSTALL_DIR)
                                    , 
                                    $(INSTALL_LIB)
                                    ,
                                    $(INSTALL_MAN)
                                    , and 
                                    $(INSTALL_SCRIPT)

                                    variables must be used when installing files so that the proper
                                    ownership and permissions are set on the installed files.

                                
                                    The 
                                    $(RANLIB)
                                     command must be run on any static
                                    libraries after installation since the symbol table is
                                    invalidated when the library is copied on some platforms.

                                

                                # 
                                    [Developer Reference](null)
                                

                                |UL|

                                
                                
                                

                            

                        
                    
                
            
            &nbsp;
        
        
            &nbsp;
        
        
            
            
                
                    
                    Comments are owned by the poster. All other content is copyright 1998-2024 by Bill Spitzak and others. This project is hosted by 
                    The FLTK Team
                    . Please report site problems to '
                    [erco@seriss.com](mailto:erco@seriss.com)
                    '.
                    
                    &nbsp;
                
            
            
        
    


