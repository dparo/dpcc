def bin2header(data, var_name='var'):
    out = []
    out.append('unsigned char {var_name}[] = {{'.format(var_name=var_name))
    l = [ data[i:i+12] for i in range(0, len(data), 12) ]
    for i, x in enumerate(l):
        line = ', '.join([ '0x{val:02x}'.format(val=ord(c)) for c in x ])
        out.append('  {line}{end_comma}'.format(line=line, end_comma=',' if i<len(l)-1 else ''))
    out.append('};')
    out.append(f'unsigned int {var_name}_len = {len(data)};')
    return '\n'.join(out)
