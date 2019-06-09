set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'
Plugin 'w0rp/ale'
"Plugin 'vim-airline/vim-airline'
Plugin 'tpope/vim-fugitive'
Plugin 'DoxygenToolkit.vim'
Plugin 'ludovicchabant/vim-gutentags'
Plugin 'ajh17/VimCompletesMe'
Plugin 'ledger/vim-ledger'
call vundle#end()            " required
filetype plugin indent on    " required


color default
colorscheme desert
syntax on

set encoding=utf-8
set number
set tabstop=4
set expandtab
set shiftwidth=4
set tabpagemax=100
set shortmess=A
set spellfile=~/SystemConfig/Config/vim-spell.utf-8.add,~/SystemConfig/Config/vim-spell-programs.add
set foldlevelstart=99 " code folding
set tags=./tags;,tags;
set spell
set completeopt-=preview "disable preview window for autocomplete

setlocal foldmethod=syntax

augroup PreviewAutocmds
  autocmd!
  autocmd WinEnter * if &previewwindow | set nospell | endif
augroup END
augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END
autocmd FileType c let b:vcm_tab_complete = "omni"

fun! IgnoreCamelCaseSpell()
  syn match functions +\s\+\<[a-z]\+\>(+ contains=@NoSpell
  syn match myExCapitalWords +\<\w\+[_0-9A-Z-]\w*\>+ contains=@NoSpell
  syn match path +[:/-]\w\++ contains=@NoSpell
  "syn match path +/\w*+ contains=@NoSpell
  syn match th +-th\>+ contains=@NoSpell
  syn cluster Spell add=functions
  syn cluster Spell add=myExCapitalWords
  syn cluster Spell add=path
  syn cluster Spell add=th
  hi link cParenError None
  hi link cErrorInParen None
  hi link cErrInParen None
  hi link vimUserAttrbError None
  hi link vimOperError None
  hi link vimSubstFlagErr None
  

endfun
autocmd BufRead,BufNewFile * :call IgnoreCamelCaseSpell()



let g:airline_powerline_fonts = 1

if !exists('g:airline_symbols')
    let g:airline_symbols = {}
endif

" airline symbols
let g:airline_left_sep = ' '
let g:airline_left_alt_sep = ' '
let g:airline_right_sep = ' '
let g:airline_right_alt_sep = ' '
let g:airline_symbols.branch = ' '
let g:airline_symbols.readonly = ' '
let g:airline_symbols.linenr = ' '

let g:DoxygenToolkit_briefTag_pre=""

let g:ale_c_gcc_options = '-std=gnu11 -Werror -Wextra -Wall -Wincompatible-pointer-types -Wno-parentheses -Wno-sign-compare -Wno-missing-braces -Wno-missing-field-initializers -Wno-cast-function-type -D_DEFAULT_SOURCE'
let g:ale_fix_on_save = 1
let g:ale_fixers = {'python': ['autopep8']}
let g:ale_python_pylint_options = "-E"
let g:ale_sh_shellcheck_exclusions = "SC2068,SC2076"
let g:ale_sh_shellcheck_options="--severity=error"
let g:ale_python_flake8_options="--select=E999"
let g:ale_python_autopep8_options="--ignore E501,E711,E721,W601,W602,W603,W604"


let g:vcm_default_maps = 0

augroup resCur
  autocmd!
  autocmd BufReadPost * call setpos(".", getpos("'\""))
augroup END


command GenTags !gcc -M *.[ch] | sed -e 's/[\\ ]/\n/g' | sed -e '/^$/d' -e '/\.o:[ \t]*$/d' | sort -V |uniq | ctags -R -L - --c++-kinds=+p
command FlipEquals s/\([^= ]\+\)\(\s*\)=\(\s*\)\([^;]*\)/\4\2=\3\1
command Strip %s/\s\+$//g




imap <C-Space> <plug>vim_completes_me_forward
imap <C-@> <plug>vim_completes_me_forward
imap jj <Esc>
nmap <silent> <C-k> <Plug>(ale_previous_wrap)
nmap <silent> <C-j> <Plug>(ale_next_wrap)
nmap <CR> i<CR><Esc>
map <F10> :echo "hi<" . synIDattr(synID(line("."),col("."),1),"name") . '> trans<'
\ . synIDattr(synID(line("."),col("."),0),"name") . "> lo<"
\ . synIDattr(synIDtrans(synID(line("."),col("."),1)),"name") . ">"<CR>
