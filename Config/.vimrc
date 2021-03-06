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
Plugin 'tpope/vim-fugitive'
"Plugin 'ajh17/VimCompletesMe'
Plugin 'akhaku/vim-java-unused-imports'
Plugin 'iamcco/markdown-preview.vim'
call vundle#end()            " required
filetype plugin indent on    " required


color default
colorscheme desert
syntax on

set backspace=indent,eol,start
set encoding=utf-8
set expandtab
set foldlevelstart=99 " code folding
set foldmethod=syntax
set hidden
set laststatus=2
set noshowmode
set noswapfile
set number
set shiftwidth=4
set shortmess=A
set spell
set spellfile=~/SystemConfig/Config/vim-spell.utf-8.add,~/SystemConfig/Config/vim-spell-programs.add
set spelloptions=camel
set tabpagemax=100
set tabstop=4
set tags=./tags;,tags;
set title
set tw=0
set updatetime=1000
set wildignore=*.gc??,*.o
set wildmode=list:longest,full

let &titlestring=$SESSION_NAME . "%f"

let g:mkdp_browserfunc = 'OpenBrowser'
let g:mkdp_auto_close = 0

function! OpenBrowser(url)
        execute "! $BROWSER " a:url "& "
endfunction


augroup resCur
  autocmd!
  autocmd BufReadPost * call setpos(".", getpos("'\""))
augroup END

augroup Misc
  autocmd!
  " Resize splits when the window is resized
  au VimResized * :wincmd =
  " Don't enter insert mode for non-modifiable files
  autocmd BufRead * let &l:modifiable = !&readonly
  autocmd BufRead * if &readonly | set nospell
  autocmd BufWritePre * Strip
augroup END

function! UpdateFile()
    silent execute "update! " . expand("~/.cache/backup/%:t")
endfunction
augroup Backup
  autocmd!
    " remove temp file on save
    autocmd BufWritePost * silent execute "!rm -f " . expand("~/.cache/backup/%:t")
    " Auto backup on leaving insert
    au InsertLeave * call UpdateFile()
augroup END
" Command to recover file created with above
command! Recover execute "1,$d|0r" expand("~/.cache/backup/%:t")


augroup project
  autocmd!
  autocmd BufRead,BufNewFile *.h,*.c set filetype=c
  autocmd FileType c setlocal makeprg=tcc\ -c\ %
  autocmd FileType sh setlocal makeprg=shellcheck\ -e\ SC1090,SC2068,SC2086,SC2048\ -f\ gcc\ %
  autocmd FileType python setlocal makeprg=python-linter
  autocmd FileType json setlocal formatprg=jq\ -r\ .
  autocmd BufWritePost * if &makeprg != "make" | silent make! % | silent redraw!
  autocmd BufWritePost *.py,*.c,*.h silent !ctags -a %
  autocmd BufWritePost *.py edit | call setpos(".", getpos("'\""))
  autocmd QuickFixCmdPost [^l]* cwindow
augroup END

command! -bar -range PadOperand s/\([^ /!+=\-<>{}]\)\([/+-=><!]=\|=\|{\|?\|:\)/\1 \2/ge | s/\([/+-=><!{}]=\|=\|,\|}\|?\|:\)\([^ ,/!+=\-<>]\)/\1 \2/ge
"command! GenTags !tcc -E *.c **/*.c | grep "^\# .*\.[ch]" | awk '{print $3}' | tr -d '"' | sort -u |  xargs -I{} realpath --relative-to=. {} | sed -E "s|^(../)+|/|g" |sort -u |  ctags --kinds-c=+p -R -L -

function! GenerateTags()
    if &filetype == "c"
        !gcc -M *.[ch] | grep -E "^\s*/"  | sed -e 's/[\\ ]/\n/g' | sed -e '/^$/d' -e '/\.o:[ \t]*$/d' | sort -V | uniq | ctags -R -L - --c++-kinds=+p
    else
        !ctags -R --exclude="*cov*" --exclude="*doc*" .
    endif
endfunction
command! GenTags call GenerateTags()


command! -bar -range FlipEquals s/\([^=>< ]\+\)\(\s*\)=\(\s*\)\([^;]*\)/\4\2=\3\1
command! -bar -range Strip %s/\s\+$//ge
command! -nargs=1 -complete=file T tab drop <args>
command! -nargs=1 -complete=file Tabe tab drop <args>
command! -bar -range -nargs=1 Renum :let @a=<q-args> | '<,'>s/\d\+/\=(@a+setreg('a',@a+1))/g


" It's 2019.
" navigate visual lines
noremap j gj
noremap k gk
noremap gj j
noremap gk k

map ;; <C-w>T
noremap <Space> <C-w>w

noremap <leader>gcd :cd %:p:h<CR>
noremap <leader>lcd :lcd %:p:h<CR>

noremap <silent> <leader>yy :w !xsel -i -b <CR>
noremap <silent> <leader>pp :r !xsel -b<CR>
noremap <silent> <leader>PP :r !xsel -p<CR>

inoremap <c-z> <esc>:suspend<cr>

inoremap <A-Space> <C-x><C-o>

noremap H ^
noremap L $
" gi already moves to "last place you exited insert mode", so we'll map gI to
" something similar: move to last change
nnoremap gI `.

imap jj <Esc>
nnoremap ff gT
nnoremap fg gt
