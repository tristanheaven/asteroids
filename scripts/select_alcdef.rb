#!/usr/bin/ruby -w

# Selects the largest data block in each file, removing all others

DATA_DIR = 'ALCDEF_ALL'

Dir.glob(DATA_DIR + '/*.txt') do |filename|
  tmp = filename + '.tmp'
  lines = File.readlines(filename).to_a.chunk{ |i| i == "\n" }.to_a
  blocks = []
  lines.each {|line| blocks.push(line[1])}
  File.open(tmp, 'w') { |file| file.puts blocks.max }
  File.rename(tmp, filename)
end
