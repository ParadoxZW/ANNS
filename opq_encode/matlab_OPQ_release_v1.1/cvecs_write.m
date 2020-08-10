function cvecs_write (filename, v)

% open the file and count the number of descriptors
fid = fopen (filename, 'wb');
d = size (v, 1);
n = size (v, 2);

for i = 1:d
  
  % first write the vector size
%   count = fwrite (fid, n, 'int');

%   if count ~= 1 
%     error ('Unable to write vector dimension: count !=1 \n');
%   end

  % write the vector components
  count = fwrite (fid, v(i, :) - 1, 'uint8');

  if count ~= n
    error ('Unable to write vector elements: count !=1 \n');
  end
end

fclose (fid);
end
