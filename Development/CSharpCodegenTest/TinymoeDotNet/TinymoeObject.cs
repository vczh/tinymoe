using System;
using System.Collections.Generic;
using System.Linq;

namespace TinymoeDotNet
{
    public class TinymoeObject
    {
        private static readonly Dictionary<Tuple<Type, string>, TinymoeObject> extensions = new Dictionary<Tuple<Type, string>, TinymoeObject>();
        private static UInt64 counter = 0;
        private Dictionary<string, TinymoeObject> fields = new Dictionary<string, TinymoeObject>();
        private List<string> fieldNames = new List<string>();
        private UInt64 id = counter++;

        public static void SetExtension(Type type, string name, TinymoeObject value)
        {
            extensions[Tuple.Create(type, name)] = value;
        }

        public void SetField(string name, TinymoeObject value)
        {
            this.fields[name] = value;
            this.fieldNames.Add(name);
        }

        public TinymoeObject GetField(string name)
        {
            TinymoeObject value = null;
            if (this.fields.TryGetValue(name, out value))
            {
                return value;
            }

            Type type = GetType();
            while (type != null)
            {
                if (extensions.TryGetValue(Tuple.Create(type, name), out value))
                {
                    return value;
                }
                type = type.BaseType;
            }

            throw new ArgumentOutOfRangeException("name");
        }

        public TinymoeObject SetFields(TinymoeObject[] values)
        {
            foreach (var x in this.fieldNames.Zip(values, Tuple.Create))
            {
                this.fields[x.Item1] = x.Item2;
            }
            return this;
        }

        public UInt64 Id
        {
            get
            {
                return this.id;
            }
        }
    }

    public class TinymoeBoolean : TinymoeObject
    {
        public bool Value { get; private set; }

        public TinymoeBoolean(bool value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(boolean)" + Value.ToString();
        }
    }

    public class TinymoeInteger : TinymoeObject
    {
        public int Value { get; private set; }

        public TinymoeInteger(int value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(integer)" + Value.ToString();
        }
    }

    public class TinymoeFloat : TinymoeObject
    {
        public double Value { get; private set; }

        public TinymoeFloat(double value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(float)" + Value.ToString();
        }
    }

    public class TinymoeString : TinymoeObject
    {
        public string Value { get; private set; }

        public TinymoeString(string value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(string)" + Value.ToString();
        }
    }

    public class TinymoeSymbol : TinymoeObject
    {
        public string Value { get; private set; }

        public TinymoeSymbol(string value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(symbol)" + Value.ToString();
        }
    }

    public class TinymoeArray : TinymoeObject
    {
        public TinymoeObject[] Elements { get; private set; }

        public TinymoeArray(int length)
        {
            this.Elements = new TinymoeObject[length];
        }

        public TinymoeArray(TinymoeObject[] values)
        {
            this.Elements = values.ToArray();
        }

        public override string ToString()
        {
            return "[" + Elements.Aggregate("", (a, b) => a + (a == "" ? "" : ",") + b.ToString()) + "]";
        }
    }

    public class TinymoeFunction : TinymoeObject
    {
        public Action<TinymoeObject[]> Handler { get; private set; }

        public TinymoeFunction(Action<TinymoeObject[]> value)
        {
            this.Handler = value;
        }

        public override string ToString()
        {
            return "(function)";
        }
    }

    public class TinymoeOperations
    {
        static TinymoeOperations()
        {
        }

        public static TinymoeObject Positive(TinymoeObject a)
        {
            return CastToNumber(a);
        }

        public static TinymoeObject Negative(TinymoeObject a)
        {
            var value = CastToNumber(a);
            var f = value as TinymoeFloat;
            if (f != null)
            {
                return new TinymoeFloat(-f.Value);
            }
            else
            {
                return new TinymoeInteger(-(value as TinymoeInteger).Value);
            }
        }

        public static TinymoeObject Not(TinymoeObject a)
        {
            var value = CastToBoolean(a);
            return new TinymoeBoolean(!value.Value);
        }

        public static TinymoeObject Concat(TinymoeObject a, TinymoeObject b)
        {
            var v1 = CastToString(a);
            var v2 = CastToString(b);
            return new TinymoeString(v1.Value + v2.Value);
        }

        private static TinymoeObject DealWithNumbers(TinymoeObject a, TinymoeObject b, Func<double, double, double> fd, Func<int, int, int> fi)
        {
            if (a is TinymoeInteger)
            {
                var va = (TinymoeInteger)a;
                if (b is TinymoeInteger)
                {
                    var vb = (TinymoeInteger)b;
                    return new TinymoeInteger(fi(va.Value, vb.Value));
                }
                else
                {
                    var vb = (TinymoeFloat)b;
                    return new TinymoeFloat(fd(va.Value, vb.Value));
                }
            }
            else
            {
                var va = (TinymoeFloat)a;
                if (b is TinymoeInteger)
                {
                    var vb = (TinymoeInteger)b;
                    return new TinymoeFloat(fd(va.Value, vb.Value));
                }
                else
                {
                    var vb = (TinymoeFloat)b;
                    return new TinymoeFloat(fd(va.Value, vb.Value));
                }
            }
        }

        private static int Compare(TinymoeObject a, TinymoeObject b)
        {
            if (a is TinymoeBoolean && b is TinymoeBoolean)
            {
                var va = (TinymoeBoolean)a;
                var vb = (TinymoeBoolean)b;
                return va.Value ? vb.Value ? 0 : 1 : vb.Value ? -1 : 0;
            }
            if (a is TinymoeSymbol && b is TinymoeSymbol)
            {
                var sa = (TinymoeSymbol)a;
                var sb = (TinymoeSymbol)b;
                return string.Compare(sa.Value, sb.Value);
            }
            if (a is TinymoeInteger)
            {
                var va = (TinymoeInteger)a;
                if (b is TinymoeInteger)
                {
                    var vb = (TinymoeInteger)b;
                    return va.Value < vb.Value ? -1 : va.Value > vb.Value ? 1 : 0;
                }
                else if (b is TinymoeFloat)
                {
                    var vb = (TinymoeFloat)b;
                    return va.Value < vb.Value ? -1 : va.Value > vb.Value ? 1 : 0;
                }
            }
            else if (a is TinymoeFloat)
            {
                var va = (TinymoeFloat)a;
                if (b is TinymoeInteger)
                {
                    var vb = (TinymoeInteger)b;
                    return va.Value < vb.Value ? -1 : va.Value > vb.Value ? 1 : 0;
                }
                else if (b is TinymoeFloat)
                {
                    var vb = (TinymoeFloat)b;
                    return va.Value < vb.Value ? -1 : va.Value > vb.Value ? 1 : 0;
                }
            }

            if (a == null)
            {
                if (b == null)
                {
                    return 0;
                }
                else
                {
                    return -1;
                }
            }
            else
            {
                if (b == null)
                {
                    return 1;
                }
                else
                {
                    try
                    {
                        var sa = CastToString(a);
                        var sb = CastToString(b);
                        return string.Compare(sa.Value, sb.Value);
                    }
                    catch (Exception)
                    {
                        var ida = a.Id;
                        var idb = b.Id;
                        return ida < idb ? -1 : ida > idb ? 1 : 0;
                    }
                }
            }
        }

        public static TinymoeObject Add(TinymoeObject a, TinymoeObject b)
        {
            return DealWithNumbers(a, b, (x, y) => x + y, (x, y) => x + y);
        }

        public static TinymoeObject Sub(TinymoeObject a, TinymoeObject b)
        {
            return DealWithNumbers(a, b, (x, y) => x - y, (x, y) => x - y);
        }

        public static TinymoeObject Mul(TinymoeObject a, TinymoeObject b)
        {
            return DealWithNumbers(a, b, (x, y) => x * +y, (x, y) => x * y);
        }

        public static TinymoeObject Div(TinymoeObject a, TinymoeObject b)
        {
            var v1 = CastToFloat(a);
            var v2 = CastToFloat(b);
            return new TinymoeFloat(v1.Value / v2.Value);
        }

        public static TinymoeObject IntDiv(TinymoeObject a, TinymoeObject b)
        {
            var v1 = CastToInteger(a);
            var v2 = CastToInteger(b);
            return new TinymoeInteger(v1.Value / v2.Value);
        }

        public static TinymoeObject Mod(TinymoeObject a, TinymoeObject b)
        {
            var v1 = CastToInteger(a);
            var v2 = CastToInteger(b);
            return new TinymoeInteger(v1.Value % v2.Value);
        }

        public static TinymoeObject LT(TinymoeObject a, TinymoeObject b)
        {
            return new TinymoeBoolean(Compare(a, b) < 0);
        }

        public static TinymoeObject GT(TinymoeObject a, TinymoeObject b)
        {
            return new TinymoeBoolean(Compare(a, b) > 0);
        }

        public static TinymoeObject LE(TinymoeObject a, TinymoeObject b)
        {
            return new TinymoeBoolean(Compare(a, b) <= 0);
        }

        public static TinymoeObject GE(TinymoeObject a, TinymoeObject b)
        {
            return new TinymoeBoolean(Compare(a, b) >= 0);
        }

        public static TinymoeObject EQ(TinymoeObject a, TinymoeObject b)
        {
            return new TinymoeBoolean(Compare(a, b) == 0);
        }

        public static TinymoeObject NE(TinymoeObject a, TinymoeObject b)
        {
            return new TinymoeBoolean(Compare(a, b) != 0);
        }

        public static TinymoeObject And(TinymoeObject a, TinymoeObject b)
        {
            var v1 = CastToBoolean(a);
            var v2 = CastToBoolean(b);
            return new TinymoeBoolean(v1.Value && v2.Value);
        }

        public static TinymoeObject Or(TinymoeObject a, TinymoeObject b)
        {
            var v1 = CastToBoolean(a);
            var v2 = CastToBoolean(b);
            return new TinymoeBoolean(v1.Value || v2.Value);
        }

        public static void Invoke(TinymoeObject function, TinymoeObject[] arguments)
        {
            ((TinymoeFunction)function).Handler(arguments);
        }

        public static TinymoeObject ArrayLength(TinymoeObject array)
        {
            return new TinymoeInteger(((TinymoeArray)array).Elements.Length);
        }

        public static TinymoeObject ArrayGet(TinymoeObject array, TinymoeObject index)
        {
            return ((TinymoeArray)array).Elements[CastToInteger(index).Value];
        }

        public static void ArraySet(TinymoeObject array, TinymoeObject index, TinymoeObject value)
        {
            ((TinymoeArray)array).Elements[CastToInteger(index).Value] = value;
        }

        private static readonly Dictionary<string, TinymoeObject> externalFunctions = new Dictionary<string, TinymoeObject>();

        public static TinymoeObject BuildExternalFunction(Func<TinymoeObject[], TinymoeObject> function)
        {
            return new TinymoeFunction(__args__ =>
            {
                var result = function(__args__.Skip(1).Take(__args__.Length - 2).ToArray());
                Invoke(__args__[2], new TinymoeObject[] { __args__[0], result });
            });
        }

        public static TinymoeObject GetExternalFunction(TinymoeObject name)
        {
            var realName = ((TinymoeString)CastToString(name)).Value;
            TinymoeObject function = null;
            if (!externalFunctions.TryGetValue(realName, out function))
            {
                switch (realName)
                {
                    case "CastToBoolean":
                        function = BuildExternalFunction(__args__ => CastToBoolean(__args__[0]));
                        break;
                    case "CastToInteger":
                        function = BuildExternalFunction(__args__ => CastToInteger(__args__[0]));
                        break;
                    case "CastToFloat":
                        function = BuildExternalFunction(__args__ => CastToFloat(__args__[0]));
                        break;
                    case "CastToString":
                        function = BuildExternalFunction(__args__ => CastToString(__args__[0]));
                        break;
                    case "Print":
                        function = BuildExternalFunction(__args__ => Print(__args__[0]));
                        break;
                    case "Sqrt":
                        function = BuildExternalFunction(__args__ => Sqrt(__args__[0]));
                        break;
                    default:
                        throw new IndexOutOfRangeException(string.Format("External function \"{0}\" does not exist.", realName));
                }
                externalFunctions.Add(realName, function);
            }
            return function;
        }

        public static TinymoeBoolean CastToBoolean(TinymoeObject a)
        {
            return (TinymoeBoolean)a;
        }

        public static TinymoeObject CastToNumber(TinymoeObject a)
        {
            if (a is TinymoeInteger || a is TinymoeFloat)
            {
                return a;
            }
            else
            {
                var value = (TinymoeString)a;
                int i;
                if (int.TryParse(value.Value, out i))
                {
                    return new TinymoeInteger(i);
                }
                else
                {
                    return new TinymoeFloat(double.Parse(value.Value));
                }
            }
        }

        public static TinymoeInteger CastToInteger(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return (TinymoeInteger)a;
            }
            else if (a is TinymoeFloat)
            {
                return new TinymoeInteger((int)((TinymoeFloat)a).Value);
            }
            else
            {
                return new TinymoeInteger(int.Parse(((TinymoeString)a).Value));
            }
        }

        public static TinymoeFloat CastToFloat(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return new TinymoeFloat((double)((TinymoeInteger)a).Value);
            }
            else if (a is TinymoeFloat)
            {
                return (TinymoeFloat)a;
            }
            else
            {
                return new TinymoeFloat(double.Parse(((TinymoeString)a).Value));
            }
        }

        public static TinymoeString CastToString(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return new TinymoeString(((TinymoeInteger)a).Value.ToString());
            }
            else if (a is TinymoeFloat)
            {
                return new TinymoeString(((TinymoeFloat)a).Value.ToString());
            }
            else
            {
                return (TinymoeString)a;
            }
        }

        public static TinymoeObject Print(TinymoeObject a)
        {
            var value = CastToString(a);
            Console.WriteLine(value.Value);
            return null;
        }

        public static TinymoeObject Sqrt(TinymoeObject a)
        {
            var value = CastToFloat(a);
            return new TinymoeFloat(Math.Sqrt(value.Value));
        }
    }
}
