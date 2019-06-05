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
call vundle#end()            " required
filetype plugin indent on    " required


color default
syntax enable

set encoding=utf-8
set number
set tabstop=4
set expandtab
set shiftwidth=4
set tabpagemax=100
set shortmess=A
set spellfile=~/SystemConfig/Config/vim-spell.utf-8.add
set foldlevelstart=99 " code folding
set tags=./tags;,tags;

setlocal foldmethod=syntax

augroup autoSpell
    autocmd!
    autocmd FileType c,markdown,vim setlocal spell
augroup END
augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen
augroup END
autocmd FileType c let b:vcm_tab_complete = "omni"

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
let g:ale_fix_on_save = 0
let g:ale_fixers = {'python': ['autopep8']}
let g:ale_python_pylint_options = "-E"
let g:ale_sh_shellcheck_exclusions = "SC2068,SC2076"
let g:ale_sh_shellcheck_options="--severity=error"
let g:vcm_default_maps = 0

" restore cursor position
au BufWinLeave *.c mkview
au BufWinEnter *.c silent loadview

command GenTags !gcc -M *.[ch] | sed -e 's/[\\ ]/\n/g' | sed -e '/^$/d' -e '/\.o:[ \t]*$/d' | sort -V |uniq | ctags -R -L - --c++-kinds=+p
command FlipEquals s/\([^= ]\+\)\(\s*\)=\(\s*\)\([^;]*\)/\4\2=\3\1
command Strip %s/\s\+$//g
command IndentSort !indent-sort




imap <C-Space> <plug>vim_completes_me_forward
imap <C-@> <plug>vim_completes_me_forward
imap jj <Esc>
nmap <silent> <C-k> <Plug>(ale_previous_wrap)
nmap <silent> <C-j> <Plug>(ale_next_wrap)
nmap <CR> i<CR><Esc>
map <C-n> :NERDTreeToggle<CR>
