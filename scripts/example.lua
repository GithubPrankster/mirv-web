function init()
    print("Sup world.", math.pow(2, 10))
end

function update(t)
    scr_clean(math.abs(math.sin(t)), 0.4, 0.9, 1.0)
end
