EXTRA_DIST = jags_user_manual.tex jags_developer_manual.tex jags_install_manual.tex 

docs:
	pdflatex jags_user_manual.tex
	pdflatex jags_user_manual.tex
	pdflatex jags_installation_manual.tex
	pdflatex jags_installation_manual.tex
	pdflatex jags_developer_manual.tex
	pdflatex jags_developer_manual.tex

clean:
	rm -f jags_*_manual.aux 
	rm -f jags_*_manual.toc
	rm -f jags_*_manual.log
	rm -f jags_*_manual.pdf
