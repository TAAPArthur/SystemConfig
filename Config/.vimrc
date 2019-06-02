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
Plugin 'vim-airline/vim-airline'
Plugin 'tpope/vim-fugitive'
Plugin 'scrooloose/nerdtree'
Plugin 'DoxygenToolkit.vim'
Plugin 'ludovicchabant/vim-gutentags'
Plugin 'ajh17/VimCompletesMe'
Plugin 'ledger/vim-ledger'
Plugin 'obreitwi/vim-sort-folds'
call vundle#end()            " required
filetype plugin indent on    " required


color default
set encoding=utf-16
set noshowmode
set number
set tabstop=4
set expandtab
set shiftwidth=4
set tabpagemax=100
set showtabline=2
syntax enable
setlocal foldmethod=syntax
set tags=./tags;,tags;

let g:ale_c_gcc_options = '-std=gnu11 -Wextra -Wall -Wno-parentheses -Wno-sign-compare -Wno-missing-braces -Wno-missing-field-initializers -Wno-cast-function-type -D_DEFAULT_SOURCE'
let g:ale_python_pylint_options = "-E"
let g:ale_fixers = {'python': ['autopep8']}
let g:ale_fix_on_save = 0

let g:ale_sh_shellcheck_options="--severity=error"
let g:ale_sh_shellcheck_exclusions = "SC2068,SC2076"

let g:DoxygenToolkit_briefTag_pre=""

let g:tagbar_show_linenumbers = 0
let g:vcm_default_maps = 0

augroup resCur
  autocmd!
  autocmd BufReadPost * call setpos(".", getpos("'\""))
augroup END

command GenTags !gcc -M *.[ch] | sed -e 's/[\\ ]/\n/g' | sed -e '/^$/d' -e '/\.o:[ \t]*$/d' | sort -V |uniq | ctags -R -L - --c++-kinds=+p
command FlipEquals s/\([^= ]\+\)\(\s*\)=\(\s*\)\([^;]*\)/\4\2=\3\1
command Strip %s/\s\+$//g


  
filetype plugin on
imap <C-Space> <plug>vim_completes_me_forward
imap <C-@> <plug>vim_completes_me_forward
imap jj <Esc>
nmap <silent> <C-k> <Plug>(ale_previous_wrap)
nmap <silent> <C-j> <Plug>(ale_next_wrap)
nmap <CR> i<CR><Esc>
map <C-n> :NERDTreeToggle<CR>
