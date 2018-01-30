%% 
%% This is file `MFPREPORT.cls', based on the class file `GAUBM.cls'
%% 
%% Copyright (C) 2005-09 by Simon Dreher
%% Adapted to Bachelor and Master theses at the department 
%% for Physics of the University of Goettingen by Prof Pruschke
%% and changed for personal fits by Jan Grieb into
%% "laboratory protocol and report class"
%% Minor changes to yield the given layout for the cover page
%% by Thomas Kurz
%% 
%% This work may be distributed and/or modified under the
%% conditions of the LaTeX Project Public License, either
%% version 1.3 of this license or (at your option) any later
%% version. The latest version of this license is in:
%% 
%%    http://www.latex-project.org/lppl.txt
%% 
%% and version 1.3 or later is part of all distributions of
%% LaTeX version 2005/12/01 or later.
%% 
%% This work has the LPPL maintenance status 'maintained'.
%% The Current Maintainer of this work is Jan Grieb.
%% This work consists of all files listed in README.
%% 
\NeedsTeXFormat{LaTeX2e}[2005/12/01]
\ProvidesClass{MFPREPORT}
[2010/04/06 v1.0 Vorlage Master-Forschungspraktikum Versuchsauswertung]
\newif\if@englishpreamble\@englishpreambletrue
\newif\if@preamblelangdef\@preamblelangdeffalse
\newif\if@germanopt\@germanoptfalse
\newif\if@ngermanopt\@ngermanoptfalse
\newif\if@reportbased\@reportbasedfalse
\DeclareOption{noenglishpreamble}%
{\@preamblelangdeftrue\@englishpreamblefalse}
\DeclareOption{englishpreamble}%
{\@preamblelangdeftrue\@englishpreambletrue}
\DeclareOption{german}%
{\if@reportbased\PassOptionsToClass{\CurrentOption}{scrreprt}\else\PassOptionsToClass{\CurrentOption}{scrartcl}\fi\@germanopttrue}
\DeclareOption{ngerman}%
{\if@reportbased\PassOptionsToClass{\CurrentOption}{scrreprt}\else\PassOptionsToClass{\CurrentOption}{scrartcl}\fi\@ngermanopttrue}
\newif\if@twocolumn\@twocolumnfalse
\DeclareOption{twocolumn}%
{\if@reportbased\PassOptionsToClass{\CurrentOption}{scrreprt}\else\PassOptionsToClass{\CurrentOption}{scrartcl}\fi\@twocolumtrue}
\DeclareOption{reportbased}%
{\@reportbasedtrue}
\DeclareOption*{\if@reportbased\PassOptionsToClass{\CurrentOption}{scrreprt}\else\PassOptionsToClass{\CurrentOption}{scrartcl}\fi}
\ProcessOptions\relax
\if@reportbased
 \LoadClass[a4paper,fontsize=10pt,twoside,titlepage=no]{scrreprt}
\else
 \LoadClass[a4paper,fontsize=10pt,twoside]{scrartcl}
\fi
\AtBeginDocument{\RequirePackage{graphicx}\RequirePackage{datenumber}}
\RequirePackage{calc}
\def\nomname{Nomenclature}
\AfterPackage*{babel}{
  \iflanguage{german}{\@englishpreamblefalse}%
  {\if@preamblelangdef\else\@englishpreambletrue\fi}
  \addto{\captionsgerman}{\renewcommand*{\figurename}{Abb.}}
  \addto{\captionsgerman}{\renewcommand*{\tablename}{Tab.}}
  \iflanguage{ngerman}{\@englishpreamblefalse}%
  {\if@preamblelangdef\else\@englishpreambletrue\fi}
  \addto{\captionsngerman}{\renewcommand*{\figurename}{Abb.}}
  \addto{\captionsngerman}{\renewcommand*{\tablename}{Tab.}}
}
\AfterPackage*{german}{
  \@englishpreamblefalse\@germanopttrue
  \renewcommand*{\figurename}{Abb.}
  \renewcommand*{\tablename}{Tab.}
  \newcommand*{\captionsgermansav}{}
  \let\captionsgermansav\captionsgerman
  \renewcommand*{\captionsgerman}%
  {\captionsgermansav\def\figurename{Abb.}%
    \def\tablename{Tab.}}
}
\AfterPackage*{ngerman}{
  \@englishpreamblefalse\@ngermanopttrue
  \renewcommand*{\figurename}{Abb.}
  \renewcommand*{\tablename}{Tab.}
  \newcommand*{\captionsngermansav}{}
  \let\captionsngermansav\captionsngerman
  \renewcommand*{\captionsngerman}%
  {\captionsngermansav\def\figurename{Abb.}%
    \def\tablename{Tab.}}
}

% makros for title-options
%protocol author - example usage: \ProtocolAuthor{Markus}{Mustermann}{Markus.Mustermann@anywhere.com}
\def\ProtocolAuthor#1#2#3{\author{#1 #2} \gdef\author@thanks{#3}}
\def\author@thanks{\@latex@error{No \noexpand\author@thanks given}\@ehc}

% acronym and name of experiment - example usage:
% \LabratoryName{AG.TIM}{Numerische Simulationen von Turbulenz im interstellaren Medium}
\def\LabratoryName#1#2{%
  \gdef\lab@num{#1}%
  \title{#2}%
}
\def\prot@num{\@latex@error{No \noexpand\prot@num given}\@ehc}

% research focus - example usage: \ResearchFocus{Astro- und Geophysik (M.phy.401)}
\def\ResearchFocus#1{\gdef\@ResearchFocus{#1}}
\def\@researchfocus{\@latex@error{No \noexpand\ResearchFocus given}\@ehc}

% mentoring institute - example usage: \Institute{Institut f�r Astrophysik}
%\def\Institute#1{\gdef\@institute{#1}}
%\def\@institute{\@latex@error{No \noexpand\Institute given}\@ehc}

% mentoring assistant - example usage:
% \Assistant{Dr. Wolfram}{Schmidt}{schmidt@astro.physik.uni-goettingen.de}
\def\Assistant#1#2{\gdef\@assistant{#1 #2}}
\def\@assistant{\@latex@error{No \noexpand\Assistant given}\@ehc}
%\def\assist@thanks{\@latex@error{No \noexpand\assist@thanks given}\@ehc}

% collaboraton with whom - example usage:
% \Collaborator{Henrike}{Fleischhack}{henrike.fleischhack@googlemail.com}
%\def\Collaborator#1#2#3{\gdef\@collaborator{#1 #2} \gdef\collab@thanks{#3}}
%\def\@collaborator{\@latex@error{No \noexpand\Collaborator given}\@ehc}
%\def\collab@thanks{\@latex@error{No \noexpand\collab@thanks given}\@ehc}

% experiment conducted on which date - example usage: \ConductedOn{15}{4}{2009}
\def\ConductedOn#1#2#3{\setmydatenumber{@countconductedon}{#3}{#2}{#1}}
\newcounter{@countconductedon}

% give either \CopyWanted or \CopyNotWanted to mark the correct sign
\def\CopyWanted{\gdef\SignCopyWanted{\CheckedBox} \gdef\SignCopyUnwanted{\Square}}
\def\CopyNotWanted{\gdef\SignCopyWanted{\Square} \gdef\SignCopyUnwanted{\CheckedBox}}
\def\SignCopyWanted{\CheckedBox}
\def\SignCopyUnwanted{\Square}

% the date of submission should be given using the normal \date comand
% example usage: \date{\today}

\titlehead{\titlefont\LARGE\@subject \par Universit\"at G\"ottingen -- Fakult\"at f\"ur Physik  }
\def\s@bject{%
  \if@englishpreamble{Master Advanced Lab Course}\else{Master-Forschungspraktikum}\fi%
}
\subject{\titlefont{\s@bject}}
\date{}

\def\changemargin#1#2{\list{}{\rightmargin#2\leftmargin#1}\item[]}
\let\endchangemargin=\endlist 

\renewcommand{\@fnsymbol}[1]{\ifcase#1\or \textasteriskcentered\or \textasteriskcentered\textasteriskcentered\or \textasteriskcentered\textasteriskcentered\textasteriskcentered \or \textasteriskcentered\textasteriskcentered\textasteriskcentered\textasteriskcentered \else\@ctrerr\fi}

\def\@experiment{%
  \if@englishpreamble{Report on\par the experiment}\else{Ausarbeitung\par zum Versuch}\fi%
}

\renewcommand*\maketitle[1][1]{\begin{titlepage}%
    %\enlargethispage{2cm}
    \setcounter{page}{#1}
%    \let\footnotesize\small
%    \let\footnoterule\relax
%    \let\footnote\thanks
%    \renewcommand*\thefootnote{\@fnsymbol\c@footnote}%
    \let\@oldmakefnmark\@makefnmark
    \renewcommand*{\@makefnmark}{\rlap\@oldmakefnmark}%
    \ifx\@titlehead\@empty \else
    \noindent\begin{minipage}[t]{\textwidth}\begin{center}
      \@titlehead
    \end{center}\end{minipage}\hrule\par
    \fi
    \null\vfill
    \begin{center}
      {\titlefont\huge \@experiment{} \lab@num\par}
      \vskip 3em
      {\titlefont\huge \@title\par}
      \vskip 3em
    \end{center}\par
    \noindent%
    \if@englishpreamble
    \noindent
%    \begin{center}\Large
%      conducted on\dateselectlanguage{english}\setdatebynumber{\the@countconductedon}\datedate{} at the \@institute
%    \end{center}
    \vfill\null
    \noindent %
     \sffamily\large
     \begin{tabular}[t]{p{0.45\linewidth}p{0.5\linewidth}}
       Name:&\@author\\[2mm]
       Email: & {\author@thanks}\\[2mm]
      {Conducted on} & \dateselectlanguage{english}\setdatebynumber{\the@countconductedon}\datedate{}\\[2mm]
      {Assistant:} & \@assistant\\[2mm]
      {\normalsize Copy of document requested:} & {\normalsize \SignCopyWanted \hspace{0.25cm} yes  \hspace{1cm}\SignCopyUnwanted \hspace{0.25cm} no}\\[2mm]
        Unterschrift:&\\
     \end{tabular}
     \vspace{15mm}
     \begin{center}
     {\bfseries\Large Submission}
     \end{center}
     \begin{tabular}[t]{p{0.45\linewidth}p{0.5\linewidth}}
      Date: & Signature of assistant:\\
     \end{tabular}
     \vspace{15mm}
     \begin{center}
     {\bfseries\Large Review}
     \end{center}
     \begin{tabular}[t]{p{0.45\linewidth}p{0.5\linewidth}}
      Date: & Name of examiner:\\[5mm]
      Points: & Signature:\\[5mm]
      Mark:       &\\[15mm]
     \end{tabular}
     \else
    {
     \if@ngermanopt\selectlanguage{ngerman}%
     \else\if@germanopt\selectlanguage{german}%
     \fi%
     \fi%
      \noindent%
%      \begin{center}\Large
%        durchgef�hrt am \dateselectlanguage{ngerman}\setdatebynumber{\the@countconductedon}\datedate{} am \@institute
%      \end{center}
      \vfill\null
      \noindent %
     \sffamily\large
     \begin{tabular}[t]{p{0.45\linewidth}p{0.5\linewidth}}
      Name: &{\@author}\\[2mm]
      Email: & {\author@thanks}\\[2mm]
     {Datum Versuchsdurchf\"uhrung:} & \dateselectlanguage{ngerman}\setdatebynumber{\the@countconductedon}\datedate{}\\[2mm]
%       \bfseries Mitarbeit:&\@collaborator\thanks{\collab@thanks}{ }&&\\&&&\\
      {Name Betreuer(in):} & \@assistant\\[2mm]
      {\normalsize Kopie der testierten Ausarbeitung gew\"unscht:} & {\normalsize \SignCopyWanted \hspace{0.25cm} ja  \hspace{1cm}\SignCopyUnwanted \hspace{0.25cm} nein}\\[2mm]
       Unterschrift:&\\
     \end{tabular}
     \vspace{15mm}
     \begin{center}
     {\bfseries\Large Abgabe}
     \end{center}
     \begin{tabular}[t]{p{0.45\linewidth}p{0.5\linewidth}}
      Datum: & Unterschrift Betreuer(in):\\
     \end{tabular}
     \vspace{15mm}
     \begin{center}
     {\bfseries\Large Testat}
     \end{center}
     \begin{tabular}[t]{p{0.45\linewidth}p{0.5\linewidth}}
      Datum: & Name Pr\"ufer(in):\\[5mm]
      Punktezahl: & Unterschrift:\\[5mm]
      Note:       &\\[15mm]
     \end{tabular}
    }
    \fi
%    \@thanks
%    \vfill
    \null
    \if@twoside\next@tpage
     \begin{minipage}[t]{\textwidth}
        \@uppertitleback
      \end{minipage}\par
      \vfill
      \begin{minipage}[b]{\textwidth}
        \@lowertitleback
      \end{minipage}
    \fi
    \ifx\@dedication\@empty \else
    \next@tpage\null\vfill
%    {\centering \Large \@dedication \par}
    \begin{changemargin}{1.7in}{0in}
     {\flushright \large \@dedication \par}
    \end{changemargin}
    \vskip \z@ \@plus3fill
    \if@twoside \next@tpage\cleardoublepage \fi
    \fi
  \end{titlepage}

  \setcounter{footnote}{0}%

  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@thanks\@empty
  \global\let\author@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\prot@num\@empty
  \global\let\@title\@empty
  \global\let\@titlehead\@empty
  \global\let\@subject\@empty
  \global\let\author\relax
  \global\let\title\relax
  \global\let\titlehead\relax
  \global\let\subject\relax
  \global\let\date\relax
  \global\let\and\relax}

\newif\if@hyperref\@hyperreffalse
\AfterPackage*{hyperref}{%
  \@hyperreftrue
  \newcommand{\org@maketitle}{}%
  \let\org@maketitle\maketitle
  \def\maketitle{%
    \hypersetup{
      %unicode=true, %% leads to an error: unicode not avaiable for bookmarks !?!
      pdftitle={\@title},
      pdfauthor={\@author},
      pdfsubject={\s@bject},
      %% FIXME: something more useful would be nice
      pdfcreator={\@author},
      pdfkeywords={FIXME}
    }%
    \org@maketitle
  }
}

% is this reportbased
\newcommand{\ifreport}[1]{\if@reportbased #1 \fi}
\newcommand{\ifreportelse}[2]{\if@reportbased #1 \else #2 \fi}

\endinput
%% 
%% End of file `MFPREPORT.cls'.
