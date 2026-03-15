

function SandR
{
	what="${1}"
	with="${2}"

	sedr="s/${what}/${with}/g"
	find . -type f -not -path '*/\.*' -exec sed -i ${sedr} {} +
	
}

