\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{marvelous}[2010/09/01 The marvelous document class by Sebastien Aupetit <aupetit@univ-tours.fr>, V1.0]

\RequirePackage{etex}              
\RequirePackage{etoolbox}
\RequirePackage{calc}

%-------------- Options ----------------
\newbool{marvel@french}
\DeclareOption{french}{\setbool{marvel@french}{true}}

\newbool{marvel@overfullbox}
\DeclareOption{overfullbox}{\setbool{marvel@overfullbox}{true}}

\newbool{marvel@beamer}
\DeclareOption{beamer}{\setbool{marvel@beamer}{true}}

\newbool{marvel@blackandwhite}
\DeclareOption{blackandwhite}{\setbool{marvel@blackandwhite}{true}}

\newbool{marvel@correction}
\DeclareOption{correction}{\setbool{marvel@correction}{true}}

\newbool{marvel@numberedspecialblock}
\DeclareOption{numberedspecialblock}{\setbool{marvel@numberedspecialblock}{true}}

\newbool{marvel@openany}
\DeclareOption{openany}{\setbool{marvel@openany}{true}}
 
\DeclareOption*{\PackageWarning{marvelous}{Unknown option `\CurrentOption'}}

\ProcessOptions\relax


%------------- global lrbox (use \begin{lrbox*} instead of \begin{lrbox} to save the box globally instead of locally ----------------
\cslet{lrbox*}\lrbox
\expandafter\patchcmd\csname lrbox*\endcsname{\setbox}{\global\setbox}{}{}
%\expandafter\show\csname lrbox*\endcsname % uncomment to see if it has worked
\cslet{endlrbox*}\endlrbox


%-------------- Configuration ----------------

\PassOptionsToPackage{svgnames,table,override}{xcolor}

\ifbool{marvel@french}{
    \PassOptionsToPackage{french}{babel}  % french document
    \RequirePackage{babel}
    \frenchbsetup{StandardEnumerateEnv=true}
    
}{
    \PassOptionsToPackage{english}{babel} % english document
    \RequirePackage{babel}
}





\ifbool{marvel@beamer}
{
    \LoadClass[hyperref,9pt]{beamer}
}{
    \ifbool{marvel@openany}{\PassOptionsToClass{openany}{book}}{}
    \PassOptionsToClass{final}{book}
    \LoadClass[10pt,twoside]{book}
    
    \RequirePackage[rigidchapters,explicit]{titlesec}
    
    % dimension de la page
    % Taille de la Page     15.24cm x 22.86cm
    % Marges    1.25cm (1.5875cm sur le cÃŽtÃ© intÃ©rieur)
    
    \setlength\paperheight{29.7cm}%
    \setlength\paperwidth{21cm}%
    
    %16.828cm x 26cm
    %21cm x 29.7cm
    %21cm x 21*26/16.828=32.45cm
    
    %\setlength\paperheight{32.45cm}%
    %\setlength\paperwidth{21cm}%
    
    
    \RequirePackage[
            verbose,
            papersize={\paperwidth,\paperheight},
            twoside,
            bindingoffset=0cm,
            heightrounded,
            includeall,
            nomarginpar,
            headheight=14pt,
            headsep=8pt,
            footskip=17pt,
            vmargin=1.5cm,
            hmargin=2cm]{geometry}
}


 




% enable overbox display as black box. must be after class loading
\ifbool{marvel@overfullbox}{
    \setlength{\overfullrule}{5pt}
}{}

\RequirePackage[T1]{fontenc}    % OT1 font encoding (must be before inputenc)
\RequirePackage[utf8]{inputenc} % encoding is set to utf8
\RequirePackage{textcomp}
\DeclareUnicodeCharacter{B0}{\textdegree}

\ifbool{marvel@french}{
    \RequirePackage[french]{babel}  % french document
}{
    \RequirePackage[english]{babel} % english document
}

\RequirePackage[frame]{crop}
\RequirePackage{layout}

%-------------- lot of packages -------------

\RequirePackage{fancyhdr}
\RequirePackage{caption}
\RequirePackage{makeidx}
\RequirePackage{amsmath}
\RequirePackage{amsfonts}
\RequirePackage{marvosym}
\RequirePackage{stmaryrd}
\RequirePackage{listings}
\RequirePackage{fancybox}
\RequirePackage{tabu}
\RequirePackage{lscape}
\RequirePackage{comment}
\RequirePackage{pgf}
\RequirePackage{tikz}
\RequirePackage{comment}
\RequirePackage{url}
\RequirePackage{framed}
\RequirePackage{fancybox}
\RequirePackage{fancyvrb}
\RequirePackage{tabu}
\RequirePackage{makerobust} 

\ifbool{marvel@beamer}{%
	\input{styles/def/beamerbase.def}%
        
    %\setbeamerfont{block title}{size={}}
    
    \input{styles/def/title.def}%
    \input{styles/def/beamertitle.def}%
     
    \input{styles/def/beamerinner.def}
    \input{styles/def/beamerouter.def}
    \input{styles/def/beamersectionning.def}
    \input{styles/def/beamercolors.def}
     
    \newcounter{chapter}
     
    \input{styles/def/specialblock.def}
    \input{styles/def/beamerspecialblock.def}
     
    
     
    \input{styles/def/listings.def}%   
    
    \input{styles/def/beamerspacing.def}
    
    \input{styles/def/references.def}    
}%
{%
    \input{styles/def/bookbeamercompat1.def}%
     
    \input{styles/def/title.def}%
    \input{styles/def/booktitle.def}%
    \input{styles/def/specialblock.def}
    \input{styles/def/bookspecialblocks.def}%
    \input{styles/def/bookheaderfooter.def}
    \input{styles/def/booksectionning.def}
    \input{styles/def/bookfloats.def}
    \input{styles/def/bookspacings.def}

    \setcounter{tocdepth}{3}
    \renewcommand{\@dotsep}{0.1}

    \input{styles/def/references.def}

    
  

    \input{styles/def/bookbeamercompat2.def}
    %\def\smallpresentation{\normalsize}
    %\def\verysmallpresentation{\small}

    \input{styles/def/listings.def}%
}    


\renewcommand{\baselinestretch}{1}

% table
\ifbool{marvel@blackandwhite}{
	\arrayrulecolor{black}
}
{
	\arrayrulecolor{NavyBlue}
}
\renewcommand{\arraystretch}{1.2}












 
    
	