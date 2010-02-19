EXTRA_DIST = jags_user_manual.tex jags_developer_manual.tex

docs:
	pdflatex $(top_srcdir)/doc/manual/jags_user_manual.tex
	pdflatex $(top_srcdir)/doc/manual/jags_user_manual.tex
	pdflatex $(top_srcdir)/doc/manual/jags_developer_manual.tex
	pdflatex $(top_srcdir)/doc/manual/jags_developer_manual.tex

clean:
	rm -f jags_*_manual.aux 
	rm -f jags_*_manual.toc
	rm -f jags_*_manual.log
	rm -f jags_*_manual.pdf
