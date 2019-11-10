set nocompatible              " be iMproved, required
filetype off                  " required
let mapleader = ";"
autocmd!

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
Plugin 'skywind3000/gutentags_plus'
Plugin 'ajh17/VimCompletesMe'
Plugin 'akhaku/vim-java-unused-imports'
" Plugin 'ledger/vim-ledger'
Plugin 'iamcco/markdown-preview.vim'
Plugin 'kamykn/spelunker.vim'
Plugin 'skywind3000/vim-preview'
Plugin 'vim-scripts/taglist.vim'
call vundle#end()            " required
filetype plugin indent on    " required



color default
colorscheme desert
syntax on

set title
set hidden
set ttyfast
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
"set completeopt-=preview "disable preview window for autocomplete
set laststatus=2
set wildignore=*.gc??,*.o
set updatetime=1000
set noswapfile
set foldmethod=syntax
set nospell

set title titlestring=
set title titlestring+=%<%f\ %([%{Tlist_Get_Tagname_By_Line()}]%)
"augroup PreviewAutocmds
"  autocmd!
"  autocmd WinEnter * if &previewwindow | set nospell | endif
"augroup END
let b:vcm_tab_complete = "omni"
autocmd BufWritePre * Strip
let b:spelunker_check_type = 1
autocmd BufWinEnter * if line("$") > 500 | let b:spelunker_check_type = 2 | endif

" Resize splits when the window is resized
au VimResized * :wincmd =

let g:DoxygenToolkit_briefTag_pre=""

let g:ale_cpp_gcc_options = '-x c++ -std=gnu++17 -lstdc++ -Werror -Wextra -Wall -Wincompatible-pointer-types -Wno-parentheses -Wno-sign-compare -Wno-missing-field-initializers'
let g:ale_c_gcc_options = '-std=gnu11 -Werror -Wextra -Wall -Wincompatible-pointer-types -Wno-parentheses -Wno-sign-compare -Wno-missing-braces -Wno-missing-field-initializers -Wno-cast-function-type -D_DEFAULT_SOURCE'
let g:ale_java_javac_options = '-Xlint:all'
let g:ale_fix_on_save = 1
let g:ale_fixers = {'python': ['autopep8']}
let g:ale_python_pylint_options = "-E"
let g:ale_sh_shellcheck_exclusions = "SC2068,SC2076"
let g:ale_sh_shellcheck_options="--severity=error"
let g:ale_python_flake8_options="--select=E999"
let g:ale_python_autopep8_options="--ignore E501,E711,E721,W601,W602,W603,W604"

let g:gutentags_modules = ['ctags', 'gtags_cscope']
let g:gutentags_ctags_exclude = ["doc", "*.md"]
let g:gutentags_define_advanced_commands = 1
let g:gutentags_project_root = ['.root', '.git']
let g:gutentags_plus_switch = 0

let g:vcm_default_maps = 0

augroup resCur
  autocmd!
  autocmd BufReadPost * call setpos(".", getpos("'\""))
augroup END


set tw=0

command -bar PadOperand %s/\([^ /!+=\-<>{}]\)\([/+-=><!]=\|=\|{\)/\1 \2/ge | %s/\([/+-=><!{}]=\|=\|,\|}\)\([^ ,/!+=\-<>]\)/\1 \2/ge
command GenTags !gcc -M *.[ch] | sed -e 's/[\\ ]/\n/g' | sed -e '/^$/d' -e '/\.o:[ \t]*$/d' | sort -V |uniq | ctags -R -L - --c++-kinds=+p
command GenTagspp !gcc  -std=c++11 -M *.{cpp,h} | sed -e 's/[\\ ]/\n/g' | sed -e '/^$/d' -e '/\.o:[ \t]*$/d' | sort -V |uniq | ctags -R -L - --c++-kinds=+p
command FlipEquals s/\([^=>< ]\+\)\(\s*\)=\(\s*\)\([^;]*\)/\4\2=\3\1
command -bar Strip %s/\s\+$//ge



" It's 2019.
" navigate visual lines
noremap j gj
noremap k gk
noremap gj j
noremap gk k


fu! Profile()
    :profile start profile.log
    :profile func *
    :profile file *
endfunction
fu! StopProfile()
    :profile pause
endfunction

map ;; <C-w>T
noremap <Space> <C-w>w

inoremap :w<cr> <c-o>:w<cr>
inoremap :ww :w


noremap H ^
noremap L $
" gi already moves to "last place you exited insert mode", so we'll map gI to
" something similar: move to last change
nnoremap gI `.

set noshowmode
au CursorMovedI *.cpp,*.java,.*.sh,*.md PreviewSignature
"inoremap ( (<c-\><c-o>:PreviewSignature<cr>
"inoremap hh <c-\><c-o>:PreviewSignature<cr>

imap <C-Space> <plug>vim_completes_me_forward
imap <C-@> <plug>vim_completes_me_forward
imap jj <Esc>
nnoremap ff gT
nnoremap fg gt
nnoremap <silent> gt :TlistToggle<CR>

nmap <silent> <C-k> <Plug>(ale_previous_wrap)
nmap <silent> <C-j> <Plug>(ale_next_wrap)
nmap <CR> i<CR><Esc>
map <F10> :echo "hi<" . synIDattr(synID(line("."),col("."),1),"name") . '> trans<'
\ . synIDattr(synID(line("."),col("."),0),"name") . "> lo<"
\ . synIDattr(synIDtrans(synID(line("."),col("."),1)),"name") . ">"<CR>
