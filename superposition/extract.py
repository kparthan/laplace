import sys, codecs, os, re
if sys.stdout.encoding is None:
        sys.stdout = codecs.open("/dev/stdout", "w", 'utf-8')
        
fr = open('test/lcb_results.txt','r')        
file_download = open('download.sh','w')
pdb_ids = []
file_extract = open('extract.sh','w')
line = fr.readline()
line_count = 0
pdb_files_count = 0
file_simulate = open('simulate.sh','w')
while line != '':
  x = line.strip('\n')
  y = line.split()
  pdb_id = y[0]

  # check if pdb file exists
  # if the file does not exist, download
  pdb_file_name = pdb_id + '.pdb.gz'
  pdb_file_path = 'test/pdb_files/' + pdb_file_name
  if pdb_id not in pdb_ids:
    pdb_ids.append(pdb_id)
    download_cmd = 'wget http://www.rcsb.org/pdb/files/' + pdb_file_name 
    file_download.write(download_cmd+'\n')
    mv_cmd = 'mv ' + pdb_file_name + ' ' + pdb_file_path
    file_download.write(mv_cmd+'\n')
    rm_cmd = 'rm ' + pdb_file_name
    file_download.write(rm_cmd+'\n');
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
  simulate_cmd = './superimpose --proteins test/2IC7_test.pdb ' + extracted_file_path + ' '
  simulate_cmd += '--accept 0.001 --translate 0.001 --rotate 0.05'
  file_simulate.write(simulate_cmd+'\n');
  print line_count
  line = fr.readline()

fr.close()
file_download.close()
file_extract.close()
file_simulate.close()

print "# of test cases: ", line_count
print "# of distinct PDB files: ", pdb_files_count 

