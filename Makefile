.data: ALCDEF_ALL
	scripts/squash_alcdef.sh
	ruby scripts/select_alcdef.rb
	touch .data

all: .data
