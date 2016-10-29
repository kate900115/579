#! /bin/tcsh -f
##this is a bash script
set filename_in = "s832.v"
set filename_out = "${filename_in}_modify"
set temp =  "${filename_in}_temp"
rm -rf $filename_out
rm -rf $temp

##delete the D_FF module part
if(0) then
	set begin = `cat $filename_in|grep -n -m 1 module| cut -d ':' -f1`

	#echo ${begin}

	set end = `cat $filename_in|grep -n -m 1 endmodule| cut -d ':' -f1`

	#echo ${end}

	cat $filename_in | sed "${begin},${end}d" | grep -v "//"> $temp

else
	cat $filename_in | grep -v "//" > $temp
endif

##
if(1) then
	set input_begin = `cat $temp|grep -n -m 1 input| cut -d ':' -f1`
	set input_end = `cat $temp| sed -n "${input_begin}"',$p'|grep -n -m 1 ";"|cut -d ':' -f1`
	#echo $input_begin
	#echo $input_end
	if ($input_end != 1) then
		set sum = `expr ${input_begin} + ${input_end} - 1`
		cat $temp | sed -n "${input_begin},${sum}p" |sed -e 's/^ *//'|sed 's/ *$//'|tr '\n' ' ' |sed -e 's/\,/ /g'|sed -e 's/;//g' | sed -e 's/ */ /'> $filename_out
		echo '\n' >> $filename_out
	else
		cat $temp|grep "input"|sed -e 's/^ *//'|sed -e 's/\,/ /g'|sed -e 's/;//g' > $filename_out
	endif
endif


if(1) then
	set output_begin = `cat $temp|grep -n -m 1 output| cut -d ':' -f1`
	set output_end = `cat $temp| sed -n "${output_begin}"',$p'|grep -n -m 1 ";"|cut -d ':' -f1`
	if ($output_end != 1) then
		set sum = `expr ${output_begin} + ${output_end} - 1`
		cat $temp | sed -n "${output_begin},${sum}p" |sed -e 's/^ *//'|sed 's/ *$//'|tr '\n' ' ' |sed -e 's/\,/ /g'|sed -e 's/;//g' | sed -e 's/ */ /'>> $filename_out
		echo '\n' >> $filename_out
	else
		cat $temp|grep "output"|sed -e 's/^ *//'|sed -e 's/\,/ /g'|sed -e 's/;//g' >> $filename_out
	endif
endif


if (1) then

	set wire_begin = `cat $temp|grep -n -m 1 wire| cut -d ':' -f1`
	set wire_end = `cat $temp| sed -n "${wire_begin}"',$p'|grep -n -m 1 ";"|cut -d ':' -f1`
	if ($wire_end != 1) then
		set sum = `expr ${wire_begin} + ${wire_end} - 1` 
		cat $temp | sed -n "${wire_begin},${sum}p" |sed -e 's/^ *//'|sed 's/ *$//'|tr '\n' ' ' |sed -e 's/\,/ /g'|sed -e 's/;//g' | sed -e 's/ */ /'>> $filename_out
		echo '\n' >> $filename_out
	else
		cat $temp|grep "wire"|sed -e 's/^ *//'|sed -e 's/\,/ /g'|sed -e 's/;//g' >> $filename_out
	endif

	#echo $wire_begin
	#echo $wire_end
	#echo $sum


endif

if (1) then
	set wire_begin = `cat $temp|grep -n -m 1 wire| cut -d ':' -f1`
	set wire_end = `cat $temp| sed -n "${wire_begin}"',$p'|grep -n -m 1 ";"|cut -d ':' -f1`
	set gate_begin = `expr $wire_begin + $wire_end + 1`
	set gate_end = `cat $temp | grep -n "endmodule"|cut -d ':' -f1`
	set gate_end = `expr ${gate_end} - 1`
	
	cat $temp | sed -n "${gate_begin},${gate_end}p" |sed -e 's/;//g'|sed -e 's/(/ /g'|sed -e 's/)/ /g'|sed -e 's/,/ /g'>> $filename_out
endif


##delete GND, VDD, CK
cat $filename_out > temp

cat temp| sed -e 's/GND//g'| sed -e 's/VDD//g'| sed -e 's/CK//g' > $filename_out

rm -rf temp



#cat $temp|grep "wire"|sed -e 's/^ *//'|sed -e 's/\,/ /g'|sed -e 's/;//g' > $filename_out
