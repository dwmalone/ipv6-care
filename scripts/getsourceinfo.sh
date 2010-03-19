#/bin/bash

top_dir=$PWD

find src -maxdepth 1 -mindepth 1 -type d | while read dir
do 
	cd $dir
	num_files=$(ls -1 *.[ch] 2>/dev/null | wc -l)

	if [ $num_files -gt 0 ]
	then
		sources=$(ls *.[ch] | tr '\n' ' ')
		{
			echo "# generated by $0"
			echo "libipv6_care_$(basename $dir)_la_SOURCES = $sources" 
		} > Makefile.src_info
	fi
	cd $top_dir
done 

