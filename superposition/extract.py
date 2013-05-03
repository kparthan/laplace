import sys, codecs, os, re
if sys.stdout.encoding is None:
        sys.stdout = codecs.open("/dev/stdout", "w", 'utf-8')
        
fr = open('test/lcb_results.txt','r')        
file_download = open('download.sh','w')
file_extract = open('extract.sh','w')
line = fr.readline()
line_count = 0
pdb_files_count = 0
while line != '':
  x = line.strip('\n')
  y = line.split()
  pdb_id = y[0]

  # check if pdb file exists
  # if the file does not exist, download
  pdb_file_name = pdb_id + '.pdb.gz'
  pdb_file_path = 'test/pdb_files/' + pdb_file_name
  bool exists = os.path.exists(pdb_file_path)
  if not exists:
    download_cmd = 'wget http://www.rcsb.org/pdb/files/' + pdb_file_name + ' > ' + pdb_file_path
    file_download.write(download_cmd+'\n')
    pdb_files_count += 1

  # construct the command to extract from the pdb file
  chain = y[1]
  residue_span = re.split('[:_]',y[2])
  residue_start = residue_span[0]
  residue_end = residue_span[2]
  extracted_file_name = pdb_id + '_' + chain + '_' + residue_start + '-' + residue_end + '.pdb'
  extracted_file_path = 'test/extracted_pdb_files/' + extracted_file_name
  extract_cmd = 'pdb-extract structure --chain ' + chain + ' '
  extract_cmd += '--residue ' + residue_start + '-' + residue_end + ' ' + pdb_file_path
  extract_cmd += ' > ' + extracted_file_path
  file_extract.write(extract_cmd+'\n')
	line_count += 1

fw.close()
fr.close()

print "# of test cases: " + line_count
print "# of distinct PDB files: " + pdb_files_count 

